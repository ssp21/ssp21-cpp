#ifndef SSP21PROXY_UDPPROXYCONFIG_H
#define SSP21PROXY_UDPPROXYCONFIG_H

#include "ProxyConfig.h"

struct Endpoint {

    Endpoint(const std::string& ip_address, uint16_t port) :
        ip_address(ip_address),
        port(port)
    {}

    const std::string ip_address;
    const uint16_t port;
};

struct UdpProxyConfig : public ProxyConfig
{
    UdpProxyConfig(
        const stack_factory_t& factory,
        const std::string& id,
        log4cpp::LogLevels log_levels,
        EndpointMode endpoint_mode,
        const Endpoint& raw_tx_endpoint,
        const Endpoint& raw_rx_endpoint,
        const Endpoint& secure_tx_endpoint,
        const Endpoint& secure_rx_endpoint
    ) :
        ProxyConfig(factory, id, log_levels, ProxyConfig::ProtoType::udp, endpoint_mode),
        raw_tx_endpoint(raw_tx_endpoint),
        raw_rx_endpoint(raw_rx_endpoint),
        secure_tx_endpoint(secure_tx_endpoint),
        secure_rx_endpoint(secure_rx_endpoint)
    {}

    const Endpoint raw_tx_endpoint;
    const Endpoint raw_rx_endpoint;
    const Endpoint secure_tx_endpoint;
    const Endpoint secure_rx_endpoint;
};

#endif
