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
        uint16_t listen_receive_port,
        uint16_t listen_send_port,
        const std::string& listen_endpoint,
        uint16_t destination_receive_port,
        uint16_t destination_send_port,
        const std::string& destination_endpoint
    ) :
        ProxyConfig(factory, id, log_levels, ProxyConfig::ProtoType::udp, endpoint_mode),
        listen_receive_port(listen_receive_port),
        listen_send_port(listen_send_port),
        listen_endpoint(listen_endpoint),
        destination_receive_port(destination_receive_port),
        destination_send_port(destination_send_port),
        destination_endpoint(destination_endpoint)
    {}

    const uint16_t listen_receive_port;
    const uint16_t listen_send_port;
    const std::string listen_endpoint;

    const uint16_t destination_receive_port;
    const uint16_t destination_send_port;
    const std::string destination_endpoint;
};

#endif
