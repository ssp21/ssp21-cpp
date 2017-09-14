
#ifndef SSP21_RESPONDERHANDSHAKES_H
#define SSP21_RESPONDERHANDSHAKES_H

#include "ssp21/crypto/IResponderHandshake.h"
#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/crypto/StaticKeys.h"

#include "openpal/logging/Logger.h"

#include <memory>

namespace ssp21
{

    /**
    * Factory class for different responder handshake patterns
    */
    struct ResponderHandshakes : private openpal::StaticOnly
    {
        static std::shared_ptr<IResponderHandshake> shared_secret_mode(
            const openpal::Logger& logger,
            const std::shared_ptr<const SymmetricKey>& shared_secret
        );

        static std::shared_ptr<IResponderHandshake> public_key_mode(
            const openpal::Logger& logger,
            const StaticKeys& static_keys,
            const std::shared_ptr<ICertificateHandler>& cert_handler
        );
    };



}

#endif
