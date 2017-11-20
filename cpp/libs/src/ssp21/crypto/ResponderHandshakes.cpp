
#include "ssp21/crypto/ResponderHandshakes.h"

#include "ssp21/crypto/PublicKeyResponderHandshake.h"
#include "ssp21/crypto/SharedSecretResponderHandshake.h"
#include "ssp21/crypto/StaticKeyLookup.h"

namespace ssp21
{
    std::shared_ptr<IResponderHandshake> ResponderHandshakes::shared_secret_mode(
        const openpal::Logger& logger,
        const std::shared_ptr<const SymmetricKey>& shared_secret
    )
    {
        return SharedSecretResponderHandshake::make_shared(logger, StaticKeyLookup::create(logger, shared_secret));
    }

    std::shared_ptr<IResponderHandshake> ResponderHandshakes::public_key_mode(
        const openpal::Logger& logger,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& cert_handler)
    {
        return PublicKeyResponderHandshake::make_shared(logger, static_keys, cert_handler);
    }
}


