
#include "StaticKeyLookup.h"

#include "openpal/logging/LogMacros.h"
#include "ssp21/stack/LogLevels.h"

namespace ssp21
{

    StaticKeyLookup::StaticKeyLookup(openpal::Logger logger, const std::shared_ptr<const SymmetricKey>& static_key) :
        logger(logger),
        static_key(static_key)
    {

    }

    std::shared_ptr<const SymmetricKey> StaticKeyLookup::find_key(const seq32_t& key_id)
    {
        if (key_id.is_empty()) return this->static_key;
        else
        {
            SIMPLE_LOG_BLOCK(this->logger, levels::error, "non-empty key identifier requested for static key mode");
            return nullptr;
        }
    }

}


