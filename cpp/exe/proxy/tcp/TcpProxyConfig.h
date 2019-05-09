#ifndef SSP21PROXY_TCPPROXYCONFIG_H
#define SSP21PROXY_TCPPROXYCONFIG_H

#include "ProxyConfig.h"

struct TcpProxyConfig : public ProxyConfig
{
    TcpProxyConfig(
        const stack_factory_t& factory,
        const std::string& id,
        log4cpp::LogLevels log_levels,
        EndpointMode endpoint_mode,
        uint16_t max_sessions,
        uint16_t listen_port,
        const std::string& listen_endpoint,
        uint16_t connect_port,
        const std::string& connect_endpoint
    ) :
        ProxyConfig(factory, id, log_levels, ProxyConfig::ProtoType::tcp, endpoint_mode),
        max_sessions(max_sessions),
        listen_port(listen_port),
        listen_endpoint(listen_endpoint),
        connect_port(connect_port),
        connect_endpoint(connect_endpoint)
    {}

    const uint16_t max_sessions;

    const uint16_t listen_port;
    const std::string listen_endpoint;

    const uint16_t connect_port;
    const std::string connect_endpoint;
};

#endif
