#ifndef SSP21PROXY_TCPCONFIG_H
#define SSP21PROXY_TCPCONFIG_H

#include "IPEndpoint.h"

#include <yaml-cpp/yaml.h>

struct TcpConfig
{
    TcpConfig(const YAML::Node& node);

    const uint16_t max_sessions;

    const IPEndpoint listen;    
    const IPEndpoint connect;
};

#endif
