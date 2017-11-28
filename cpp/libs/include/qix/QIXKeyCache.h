#ifndef SSP21PROXY_QIXKEYCACHE_H
#define SSP21PROXY_QIXKEYCACHE_H

#include "IQIXFrameHandler.h"

#include "ssp21/crypto/KeyRecord.h"
#include "openpal/logging/Logger.h"

#include <mutex>
#include <map>

class QIXKeyCache : public IQIXFrameHandler
{
public:

    QIXKeyCache(const openpal::Logger& logger, size_t max_keys);

    virtual ~QIXKeyCache() {}

    virtual void handle(const QIXFrame& frame) override final;

protected:

    const size_t max_keys;
    openpal::Logger logger;

    std::mutex mutex;

    std::map<uint64_t, std::shared_ptr<const ssp21::KeyRecord>> keys;

};

#endif
