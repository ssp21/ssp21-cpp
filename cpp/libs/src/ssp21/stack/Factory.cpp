
#include "ssp21/stack/Factory.h"

#include "ssp21/stack/LinkCryptoStack.h"

namespace ssp21
{

    std::shared_ptr<IStack> Factory::responder(
        const Addresses& addresses,
        const ResponderConfig& config,
        openpal::Logger logger,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const std::shared_ptr<IResponderHandshake>& handshake)
    {
        return std::make_shared<ResponderStack>(
                   addresses,
                   config,
                   logger,
                   executor,
                   handshake
               );
    }

    std::shared_ptr<IStack> Factory::initiator(
        const Addresses& addresses,
        const InitiatorConfig& config,
        openpal::Logger logger,
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

