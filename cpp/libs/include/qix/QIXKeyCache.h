#ifndef SSP21PROXY_QIXKEYCACHE_H
#define SSP21PROXY_QIXKEYCACHE_H

#include "IQIXFrameHandler.h"
#include "QIXFrameReader.h"

#include "ssp21/crypto/KeyRecord.h"
#include "ssp21/crypto/IKeySource.h"
#include "ssp21/crypto/IKeyLookup.h"
#include "openpal/logging/Logger.h"

#include <mutex>
#include <map>

class QIXKeyCache final : public ssp21::IKeySource, public ssp21::IKeyLookup
{
public:

    QIXKeyCache(const std::string& serial_port, const openpal::Logger& logger, size_t max_keys);

    virtual ~QIXKeyCache() {}

    virtual std::shared_ptr<const ssp21::KeyRecord> consume_key() override;

    virtual std::shared_ptr<const ssp21::SymmetricKey> find_and_consume_key(uint64_t key_id) override;

protected:

    void handle(const QIXFrame& frame);

    std::mutex mutex;

    const size_t max_keys;
    openpal::Logger logger;

    std::map<uint64_t, std::shared_ptr<const ssp21::KeyRecord>> key_map;

    // destruct the reader first
    std::shared_ptr<QIXFrameReader> reader;

};

#endif
