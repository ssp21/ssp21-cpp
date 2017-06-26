#include "PluginFactory.h"

#include "ssp21/stack/Factory.h"

using namespace ssp21;
using namespace openpal;

plugin_factory_t PluginFactory::get(const ProxyConfig& config)
{
    if (config.mode == ProxyConfig::Mode::initiator)
    {
        return [cfg = config.ssp21](
                   const Logger & logger,
                   const std::shared_ptr<Executor>& exe) -> std::shared_ptr<IStack>
        {
            InitiatorConfig config; // TODO - configure based on optional settings

            return Factory::initiator(
                Addresses(cfg.remote_address, cfg.local_address),
                config,
                logger,
                exe,
                StaticKeys(
                    cfg.local_public_key,
                    cfg.local_private_key
                ),
                ICertificateHandler::preshared_key(cfg.remote_public_key)
            );
        };
    }
    else
    {
        return [cfg = config.ssp21](
                   const Logger & logger,
                   const std::shared_ptr<Executor>& exe) -> std::shared_ptr<IStack>
        {
            ResponderConfig config; // TODO - configure based on optional settings

            return Factory::responder(
                Addresses(cfg.remote_address, cfg.local_address),
                config,
                logger,
                exe,
                StaticKeys(
                    cfg.local_public_key,
                    cfg.local_private_key
                ),
                ICertificateHandler::preshared_key(cfg.remote_public_key)
            );
        };
    }
}
