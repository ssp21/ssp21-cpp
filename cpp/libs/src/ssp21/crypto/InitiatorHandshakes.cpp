
#include "InitiatorHandshakes.h"

#include "ssp21/crypto/PublicKeyInitiatorHandshake.h"
#include "ssp21/crypto/SharedSecretInitiatorHandshake.h"
#include "ssp21/crypto/StaticKeySource.h"

namespace ssp21
{
    std::shared_ptr<IInitiatorHandshake> InitiatorHandshakes::shared_secret_mode(
        const openpal::Logger& logger,
        const CryptoSuite& crypto_suite,
        const std::shared_ptr<const SymmetricKey>& shared_secret
    )
    {
        return SharedSecretInitiatorHandshake::make_shared(logger, crypto_suite, StaticKeySource::create(shared_secret));
    }

    std::shared_ptr<IInitiatorHandshake> InitiatorHandshakes::public_key_mode(
        const openpal::Logger& logger,
        const CryptoSuite& crypto_suite,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& cert_handler
    )
    {
        return PublicKeyInitiatorHandshake::make_shared(logger, static_keys, crypto_suite, cert_handler);
    }
}


