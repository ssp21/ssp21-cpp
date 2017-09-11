
#ifndef SSP21_INITIATORFACTORY_H
#define SSP21_INITIATORFACTORY_H

#include "ssp21/stack/IStack.h"

#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "ssp21/crypto/InitiatorHandshake.h"

#include "ssp21/link/Addresses.h"

#include "openpal/logging/Logger.h"
#include "openpal/executor/IExecutor.h"

#include <memory>

namespace ssp21
{
    class InitiatorFactory final
    {
    public:

        static std::shared_ptr<IStack> create(
            const Addresses& addresses,
            const InitiatorConfig& config,
            openpal::Logger logger,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const std::shared_ptr<IInitiatorHandshake>& handshake
        );
    };
}

#endif
