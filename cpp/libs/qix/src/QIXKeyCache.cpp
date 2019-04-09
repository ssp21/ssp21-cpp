#include "qix/QIXKeyCache.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/stack/LogLevels.h"

#include "log4cpp/LogMacros.h"

#include <cinttypes>
#include <functional>

class FrameHandler : public IQIXFrameHandler
{

    using frame_handler_t = std::function<void(const QIXFrame&)>;

    frame_handler_t handler;

public:

    FrameHandler(const frame_handler_t& handler) : handler(handler) {}

    virtual void handle(const QIXFrame& frame) override
    {
        this->handler(frame);
    }
};

QIXKeyCache::QIXKeyCache(const std::string& serial_port, const log4cpp::Logger& logger, size_t max_keys) :
    max_keys(max_keys),
    logger(logger)
{
    this->reader = std::make_shared<QIXFrameReader>(
                       std::make_shared<FrameHandler>([this](const QIXFrame & frame)
    {
        this->handle(frame);
    }),
    logger,
    serial_port
                   );
}

void QIXKeyCache::handle(const QIXFrame& frame)
{
    std::unique_lock<std::mutex> lock(this->mutex);

    if (!this->key_map.empty() && frame.key_id < this->key_map.rbegin()->first)
    {
        SIMPLE_LOG_BLOCK(logger, ssp21::levels::warn, "QKD receiver reboot detected, clearing all keys from memory");
        this->key_map.clear();
    }

    if (this->key_map.empty())
    {
        this->add_key_to_map(frame);
    }
    else if(frame.key_id > this->key_map.rbegin()->first)
    {
        this->add_key_to_map(frame);
    }

    if (this->key_map.size() > this->max_keys)
    {
        // discard the oldest key
        this->key_map.erase(this->key_map.begin());
    }
}

void QIXKeyCache::add_key_to_map(const QIXFrame& frame)
{
    FORMAT_LOG_BLOCK(this->logger, ssp21::levels::info, "Received new key w/ id: %" PRIu64, frame.key_id);
    this->key_map[frame.key_id] = std::make_shared<ssp21::KeyRecord>(frame.key_id, frame.key_data);
}

std::shared_ptr<const ssp21::KeyRecord> QIXKeyCache::consume_key()
{
    std::unique_lock<std::mutex> lock(this->mutex);

    if (this->key_map.empty()) return nullptr;
    auto ret = this->key_map.rbegin()->second;
    this->key_map.erase(this->key_map.rbegin()->first);
    return ret;
}

std::shared_ptr<const ssp21::SymmetricKey> QIXKeyCache::find_and_consume_key(uint64_t key_id)
{
    std::unique_lock<std::mutex> lock(this->mutex);

    const auto entry = this->key_map.find(key_id);
    if (entry == this->key_map.end()) return nullptr;
    auto key = entry->second->key;
    this->key_map.erase(entry);
    return key;
}

