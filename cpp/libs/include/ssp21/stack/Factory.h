
#ifndef SSP21_FACTORY_H
#define SSP21_FACTORY_H

#include "ssp21/stack/IStack.h"

#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "ssp21/crypto/IResponderHandshake.h"
#include "ssp21/crypto/InitiatorHandshake.h"

#include "ssp21/link/Addresses.h"

#include "openpal/logging/Logger.h"
#include "openpal/executor/IExecutor.h"

#include <memory>

namespace ssp21
{
    class Factory final
    {
    public:

        static std::shared_ptr<IStack> responder(
            const Addresses& addresses,
            const ResponderConfig& config,
            openpal::Logger logger,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const std::shared_ptr<IResponderHandshake>& handshake
        );

        static std::shared_ptr<IStack> initiator(
            const Addresses& addresses,
            const InitiatorConfig& config,
            openpal::Logger logger,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const std::shared_ptr<IInitiatorHandshake>& handshake
        );       
    };
}

#endif
