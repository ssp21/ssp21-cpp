
#include "InitiatorHandshakes.h"

#include "ssp21/crypto/PublicKeyInitiatorHandshake.h"

namespace ssp21
{
    std::shared_ptr<IInitiatorHandshake> InitiatorHandshakes::public_key_mode(
        const openpal::Logger& logger,
        const StaticKeys& static_keys,
        const CryptoSuite& crypto_suite,
        const std::shared_ptr<ICertificateHandler>& cert_handler
    )
    {
        return PublicKeyInitiatorHandshake::create_shared(logger, static_keys, crypto_suite, cert_handler);
    }
}


