
#ifndef SSP21_RESPONDERFACTORY_H
#define SSP21_RESPONDERFACTORY_H

#include "ssp21/stack/IStack.h"

#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/IResponderHandshake.h"

#include "ssp21/link/Addresses.h"

#include "openpal/logging/Logger.h"
#include "openpal/executor/IExecutor.h"

#include <memory>

namespace ssp21
{
    class ResponderFactory final : private openpal::StaticOnly
    {
    public:

        static std::shared_ptr<IStack> create(
            const Addresses& addresses,
            const ResponderConfig& config,
            openpal::Logger logger,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const std::shared_ptr<IResponderHandshake>& handshake
        );

    };
}

#endif
