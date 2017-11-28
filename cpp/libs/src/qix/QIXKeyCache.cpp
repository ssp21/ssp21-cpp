#include "qix/QIXKeyCache.h"

#include "ssp21/crypto/Crypto.h"

KeyRecord::KeyRecord(const QIXFrame& frame) : fingerprint(), key(std::make_shared<ssp21::SymmetricKey>())
{
    ssp21::HashOutput output;
    ssp21::Crypto::hash_sha256({ frame.key_data }, output);
    memcpy(&fingerprint.front(), output.as_seq(), fingerprint.size());


    memcpy(key->as_wseq(), frame.key_data, frame.key_data.length());
}

QIXKeyCache::QIXKeyCache(size_t max_keys) : max_keys(max_keys)
{}

void QIXKeyCache::handle(const QIXFrame& frame)
{
    auto record = std::make_unique<KeyRecord>(frame);

    std::unique_lock<std::mutex> lock;
    this->keys.push_back(std::move(record));
    if (this->keys.size() > this->max_keys)
    {
        this->keys.pop_front();
    }
}


