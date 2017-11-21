
#ifndef SSP21_STATICKEYLOOKUP_H
#define SSP21_STATICKEYLOOKUP_H

#include "ssp21/crypto/IKeyLookup.h"
#include "openpal/logging/Logger.h"

namespace ssp21
{
    /**
    * Interface used to lookup a shared secret key using a key identifier
    */
    class StaticKeyLookup final : public IKeyLookup
    {
    public:

        StaticKeyLookup(const openpal::Logger& logger, const std::shared_ptr<const SymmetricKey>& static_key);

        static std::shared_ptr<IKeyLookup> create(const openpal::Logger& logger, const std::shared_ptr<const SymmetricKey>& static_key)
        {
            return std::make_shared<StaticKeyLookup>(logger, static_key);
        }

        virtual std::shared_ptr<const SymmetricKey> find_key(const seq32_t& key_id) override;

    private:

        openpal::Logger logger;
        const std::shared_ptr<const SymmetricKey> static_key;
    };




}

#endif
