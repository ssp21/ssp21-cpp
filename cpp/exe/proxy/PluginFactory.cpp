#include "PluginFactory.h"

#include "ssp21/stack/ResponderFactory.h"
#include "ssp21/stack/InitiatorFactory.h"

#include "ssp21/crypto/ResponderHandshakes.h"

using namespace ssp21;
using namespace openpal;

plugin_factory_t PluginFactory::get(const ProxyConfig& config)
{
    if (config.mode == ProxyConfig::Mode::initiator)
    {
        return [cfg = config.ssp21](
                   const Logger & logger,
                   const std::shared_ptr<IExecutor>& exe) -> std::shared_ptr<IStack>
        {
            // TODO - configure based on optional settings
            InitiatorConfig config;
            CryptoSuite suite;

            return InitiatorFactory::create(
                Addresses(cfg.remote_address, cfg.local_address),
                config,
                logger,
                exe,
                std::make_shared<InitiatorHandshake>(
                    logger,
                    cfg.local_keys,
                    suite,
                    cfg.certificate_handler
                )
            );
        };
    }
    else
    {
        return [cfg = config.ssp21](
                   const Logger & logger,
                   const std::shared_ptr<IExecutor>& exe) -> std::shared_ptr<IStack>
        {
            ResponderConfig config; // TODO - configure based on optional settings

            return ResponderFactory::create(
                Addresses(cfg.remote_address, cfg.local_address),
                config,
                logger,
                exe,
                ResponderHandshakes::public_key_mode(
                    logger,
                    cfg.local_keys,
                    cfg.certificate_handler
                )
            );
        };
    }
}
