#include "UdpConfig.h"

#include "YAMLHelpers.h"

UdpConfig::UdpConfig(const YAML::Node& node)
    : raw_rx_endpoint(yaml::require(node, "raw_rx"))
    , raw_tx_endpoint(yaml::require(node, "raw_tx"))
    , secure_rx_endpoint(yaml::require(node, "secure_rx"))
    , secure_tx_endpoint(yaml::require(node, "secure_tx"))
{
}
