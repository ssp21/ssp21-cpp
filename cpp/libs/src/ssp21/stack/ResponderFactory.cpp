
#include "ssp21/stack/ResponderFactory.h"

#include "ssp21/stack/LinkCryptoStack.h"

namespace ssp21
{

    std::shared_ptr<IStack> ResponderFactory::create(
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


}

