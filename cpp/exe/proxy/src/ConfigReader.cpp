
#include "ConfigReader.h"

#include "StackConfigReader.h"

#include "YAMLHelpers.h"
#include "LogConfig.h"
#include "tcp/TcpProxySession.h"
#include "udp/UdpProxySession.h"

#include <ssp21/util/Exception.h>

using namespace ssp21;

namespace config
{   
    enum class TransportType
    {
        TCP,
        UDP
    };

    TransportType get_transport_type(const YAML::Node& node)
    {
        const auto type = yaml::require_string(node, "type");

        if (type == "tcp") {
            return TransportType::TCP;
        }

        if (type == "udp") {
            return TransportType::UDP;
        }

        throw yaml::YAMLException(node.Mark(), "Unknown transport type: ", type);
    }


    proxy_session_factory_t get_session_factory(const YAML::Node& node)
    {
        // read the logging parameters
        const LogConfig logging(node);

        // read the SSP21 parameters before we even bother with the transport
        const auto factory = config::get_stack_factory(node);

        // the yaml node under which 
        const auto transport = yaml::require(node, "transport");

        const auto type = get_transport_type(transport);

        if (type == TransportType::TCP)
        {
			if (!factory.get_uses_link_layer())
			{
				throw yaml::YAMLException(transport, "TCP transport may only be used inconjunction with the SSP21 link-layer");
			}

            TcpConfig config(transport);
            return [=](const log4cpp::Logger& logger, std::shared_ptr<exe4cpp::BasicExecutor> executor) {
                return std::make_unique<TcpProxySession>(
                    config,
                    factory,
                    executor,
                    logger.detach(logging.id, logging.levels)
                );
            };
        }
        else
        {
            UdpConfig config(transport);
            return[=](const log4cpp::Logger& logger, std::shared_ptr<exe4cpp::BasicExecutor> executor) {
                return std::make_unique<UdpProxySession>(
                    config,
                    factory,
                    executor,
                    logger.detach(logging.id, logging.levels)
                );
            };
        }
    }
}


