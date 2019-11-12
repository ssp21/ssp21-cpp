#ifndef SSP21PROXY_QIXKEYSTORE_H
#define SSP21PROXY_QIXKEYSTORE_H

#include "IQIXFrameHandler.h"
#include "QIXFrameReader.h"

#include "ssp21/crypto/KeyRecord.h"
#include "ssp21/crypto/IKeySource.h"
#include "ssp21/crypto/IKeyLookup.h"
#include "log4cpp/Logger.h"

#include <mutex>
#include <map>

class QIXKeyStore : public ssp21::IKeyLookup, public ssp21::IKeySource
{
public:
    QIXKeyStore(uint16_t max_cached_keys);

    void add_key(const QIXFrame& frame);    

    std::shared_ptr<const ssp21::KeyRecord> consume_key() override;

    std::shared_ptr<const ssp21::SymmetricKey> find_and_consume_key(uint64_t key_id) override;

protected:

    std::mutex mutex;

private:    
    const uint16_t max_cached_keys;
    std::map<uint64_t, std::shared_ptr<const ssp21::KeyRecord>> key_map;
};

#endif
