#ifndef SSP21PROXY_IPENDPOINT_H
#define SSP21PROXY_IPENDPOINT_H

#include <string>
#include <cstdint>

#include <yaml-cpp/yaml.h>

struct IPEndpoint {

    IPEndpoint(const YAML::Node& node);

    const std::string ip_address;
    const uint16_t port;
};

#endif

