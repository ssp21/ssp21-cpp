
#include "ssp21/stack/InitiatorFactory.h"

#include "ssp21/stack/LinkCryptoStack.h"

namespace ssp21
{   
    std::shared_ptr<IStack> InitiatorFactory::create(
        const Addresses& addresses,
        const InitiatorConfig& config,
        const openpal::Logger& logger,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const std::shared_ptr<IInitiatorHandshake>& handshake)
    {
        return std::make_shared<InitiatorStack>(
                   addresses,
                   config,
                   logger,
                   executor,
                   handshake
               );
    }

}

