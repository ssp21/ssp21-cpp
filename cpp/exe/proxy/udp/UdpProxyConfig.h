#ifndef SSP21PROXY_UDPPROXYCONFIG_H
#define SSP21PROXY_UDPPROXYCONFIG_H

#include "ProxyConfig.h"

struct UdpProxyConfig : public ProxyConfig
{
    UdpProxyConfig(
        const stack_factory_t& factory,
        const std::string& id,
        log4cpp::LogLevels log_levels,
        EndpointMode endpoint_mode,
        const std::string& source_receive_endpoint,
        uint16_t source_receive_port,
        const std::string& source_send_endpoint,
        uint16_t source_send_port,
        const std::string& destination_receive_endpoint,
        uint16_t destination_receive_port,
        const std::string& destination_send_endpoint,
        uint16_t destination_send_port
    ) :
        ProxyConfig(factory, id, log_levels, ProxyConfig::ProtoType::udp, endpoint_mode),
        source_receive_endpoint(source_receive_endpoint),
        source_receive_port(source_receive_port),
        source_send_endpoint(source_send_endpoint),
        source_send_port(source_send_port),
        destination_receive_endpoint(destination_receive_endpoint),
        destination_receive_port(destination_receive_port),
        destination_send_endpoint(destination_send_endpoint),
        destination_send_port(destination_send_port)
    {}

    const std::string source_receive_endpoint;
    const uint16_t source_receive_port;
    const std::string source_send_endpoint;
    const uint16_t source_send_port;

    const std::string destination_receive_endpoint;
    const uint16_t destination_receive_port;
    const std::string destination_send_endpoint;
    const uint16_t destination_send_port;
};

#endif
