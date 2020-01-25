
#ifndef SSP21_INITIATORHANDSHAKES_H
#define SSP21_INITIATORHANDSHAKES_H

#include "crypto/IInitiatorHandshake.h"
#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/crypto/IKeySource.h"
#include "ssp21/crypto/StaticKeys.h"

#include "log4cpp/Logger.h"

#include <memory>

namespace ssp21 {

/**
    * Factory class for different initiator handshake patterns
    */
struct InitiatorHandshakes : private ser4cpp::StaticOnly {
    static std::shared_ptr<IInitiatorHandshake> shared_secret_mode(
        const log4cpp::Logger& logger,
        const CryptoSuite& crypto_suite,
        const std::shared_ptr<const SymmetricKey>& key);

    static std::shared_ptr<IInitiatorHandshake> qkd_mode(
        const log4cpp::Logger& logger,
        const CryptoSuite& crypto_suite,
        const std::shared_ptr<IKeySource>& key_source);

    static std::shared_ptr<IInitiatorHandshake> public_key_mode(
        const log4cpp::Logger& logger,
        const DHCryptoSuite& crypto_suite,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& cert_handler);
};

}

#endif
