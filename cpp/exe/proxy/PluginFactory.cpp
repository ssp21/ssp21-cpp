#include "PluginFactory.h"

#include "ssp21/stack/ResponderFactory.h"
#include "ssp21/stack/InitiatorFactory.h"

#include "ssp21/crypto/ResponderHandshakes.h"

using namespace ssp21;
using namespace openpal;

plugin_factory_t PluginFactory::get(const ProxyConfig& config)
{
    if (config.mode == ProxyConfig::EndpointMode::initiator)
    {
        return [cfg = config.ssp21](
                   const Logger & logger,
                   const std::shared_ptr<IExecutor>& exe) -> std::shared_ptr<IStack>
        {
            // TODO - configure based on optional settings
            InitiatorConfig config;
            CryptoSuite suite;

			throw std::logic_error("not implemented");
        };
    }
    else
    {
        return [cfg = config.ssp21](
                   const Logger & logger,
                   const std::shared_ptr<IExecutor>& exe) -> std::shared_ptr<IStack>
        {
            ResponderConfig config; // TODO - configure based on optional settings

			// TODO
			throw std::logic_error("not implemented");
        };
    }
}
