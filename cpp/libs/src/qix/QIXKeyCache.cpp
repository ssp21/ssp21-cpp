#include "qix/QIXKeyCache.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/stack/LogLevels.h"

#include "openpal/logging/LogMacros.h"

QIXKeyCache::QIXKeyCache(const openpal::Logger& logger, size_t max_keys) : logger(logger), max_keys(max_keys)
{}

void QIXKeyCache::handle(const QIXFrame& frame)
{
	std::unique_lock<std::mutex> lock(this->mutex);

    if (!this->keys.empty() && (frame.key_id <= this->keys.front()->id))
    {
        SIMPLE_LOG_BLOCK(logger, ssp21::levels::warn, "QKD receiver reboot detected, clearing all keys from memory");
        this->keys.clear();
    }

    this->keys.push_back(
        std::make_shared<ssp21::KeyRecord>(frame.key_id, frame.key_data)
    );

    if (this->keys.size() > this->max_keys)
    {
        // discard the oldest key
        this->keys.pop_front();
    }
}


