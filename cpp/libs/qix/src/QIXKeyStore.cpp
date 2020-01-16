#include "qix/QIXKeyStore.h"

#include <log4cpp/LogMacros.h>
#include <ssp21/stack/LogLevels.h>

QIXKeyStore::QIXKeyStore(uint16_t max_cached_keys)
    : max_cached_keys(max_cached_keys)
{
}

void QIXKeyStore::add_key(const QIXFrame& frame)
{
    std::lock_guard<std::mutex> lock(this->mutex);

    // if the key id goes backward, clear the map
    if (!this->key_map.empty() && frame.key_id < this->key_map.rbegin()->first) {
        this->key_map.clear();
    }

    //put the key into the map
    this->key_map[frame.key_id] = std::make_shared<ssp21::KeyRecord>(frame.key_id, frame.key_data);

    // discard the oldest key if the cache has grown too large
    if (this->key_map.size() > this->max_cached_keys) {
        this->key_map.erase(this->key_map.begin());
    }
}

std::shared_ptr<const ssp21::KeyRecord> QIXKeyStore::consume_key()
{
    std::lock_guard<std::mutex> lock(this->mutex);

    if (this->key_map.empty()) {
        return nullptr;
    }
    const auto ret = this->key_map.rbegin()->second;
    this->key_map.erase(this->key_map.rbegin()->first);
    return ret;
}

std::shared_ptr<const ssp21::SymmetricKey> QIXKeyStore::find_and_consume_key(uint64_t key_id)
{
    std::lock_guard<std::mutex> lock(this->mutex);

    const auto entry = this->key_map.find(key_id);
    if (entry == this->key_map.end()) {
        return nullptr;
    }
    const auto key = entry->second->key;
    this->key_map.erase(entry);
    return key;
}
