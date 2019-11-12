#ifndef SSP21PROXY_UDPCONFIG_H
#define SSP21PROXY_UDPCONFIG_H

#include "IPEndpoint.h"

struct UdpConfig
{
	UdpConfig(const YAML::Node& node);

	const IPEndpoint raw_rx_endpoint;
    const IPEndpoint raw_tx_endpoint;
	const IPEndpoint secure_rx_endpoint;
    const IPEndpoint secure_tx_endpoint;
    
};

#endif
