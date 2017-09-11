
#ifndef SSP21_INITIATORHANDSHAKES_H
#define SSP21_INITIATORHANDSHAKES_H

#include "ssp21/crypto/IInitiatorHandshake.h"
#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/crypto/StaticKeys.h"

#include "openpal/logging/Logger.h"


#include <memory>

namespace ssp21
{

    /**
    * Factory class for different initiator handshake patterns
    */
    struct InitiatorHandshakes : private openpal::StaticOnly
    {
        static std::shared_ptr<IInitiatorHandshake> public_key_mode(
            const openpal::Logger& logger,
            const StaticKeys& static_keys,
            const CryptoSuite& crypto_suite,
            const std::shared_ptr<ICertificateHandler>& cert_handler
        );
    };



}

#endif
