#ifndef SSP21PROXY_QIXKEYCACHE_H
#define SSP21PROXY_QIXKEYCACHE_H

#include "IQIXFrameHandler.h"
#include "ssp21/crypto/Constants.h"
#include "ssp21/crypto/BufferTypes.h"

#include <mutex>
#include <deque>
#include <array>

struct KeyRecord
{

    typedef std::array<uint8_t, ssp21::consts::crypto::sha256_hash_output_length> fingerprint_t;

    KeyRecord(const QIXFrame& frame);

    fingerprint_t fingerprint;
    const std::shared_ptr<ssp21::SymmetricKey> key;

};

class QIXKeyCache : public IQIXFrameHandler
{
public:

    QIXKeyCache(size_t max_keys);

    virtual ~QIXKeyCache() {}

    virtual void handle(const QIXFrame& frame) override final;

protected:

    const size_t max_keys;

    std::mutex mutex;

    std::deque<std::unique_ptr<KeyRecord>> keys;

};

#endif
