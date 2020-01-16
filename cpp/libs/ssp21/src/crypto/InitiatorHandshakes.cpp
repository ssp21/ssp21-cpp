
#include "InitiatorHandshakes.h"

#include "crypto/PublicKeyInitiatorHandshake.h"
#include "crypto/QKDInitiatorHandshake.h"
#include "crypto/SharedSecretInitiatorHandshake.h"

namespace ssp21 {
std::shared_ptr<IInitiatorHandshake> InitiatorHandshakes::shared_secret_mode(
    const log4cpp::Logger& logger,
    const CryptoSuite& crypto_suite,
    const std::shared_ptr<const SymmetricKey>& key)
{
    return SharedSecretInitiatorHandshake::make_shared(logger, crypto_suite, key);
}

std::shared_ptr<IInitiatorHandshake> InitiatorHandshakes::qkd_mode(
    const log4cpp::Logger& logger,
    const CryptoSuite& crypto_suite,
    const std::shared_ptr<IKeySource>& key_source)
{
    return QKDInitiatorHandshake::make_shared(logger, crypto_suite, key_source);
}

std::shared_ptr<IInitiatorHandshake> InitiatorHandshakes::public_key_mode(
    const log4cpp::Logger& logger,
    const CryptoSuite& crypto_suite,
    const StaticKeys& static_keys,
    const std::shared_ptr<ICertificateHandler>& cert_handler)
{
    return PublicKeyInitiatorHandshake::make_shared(logger, static_keys, crypto_suite, cert_handler);
}
}
