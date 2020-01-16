
#ifndef SSP21_RESPONDERHANDSHAKES_H
#define SSP21_RESPONDERHANDSHAKES_H

#include "crypto/IResponderHandshake.h"
#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/crypto/IKeyLookup.h"
#include "ssp21/crypto/StaticKeys.h"

#include "log4cpp/Logger.h"

#include <memory>

namespace ssp21 {

/**
    * Factory class for different responder handshake patterns
    */
struct ResponderHandshakes : private ser4cpp::StaticOnly {
    static std::shared_ptr<IResponderHandshake> shared_secret_mode(
        const log4cpp::Logger& logger,
        const std::shared_ptr<const SymmetricKey>& key);

    static std::shared_ptr<IResponderHandshake> qkd_mode(
        const log4cpp::Logger& logger,
        const std::shared_ptr<IKeyLookup>& key_lookup);

    static std::shared_ptr<IResponderHandshake> public_key_mode(
        const log4cpp::Logger& logger,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& cert_handler);
};

}

#endif
