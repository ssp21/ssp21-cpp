#include "TcpConfig.h"

#include "YAMLHelpers.h"

TcpConfig::TcpConfig(const YAML::Node& node) : 
    max_sessions(yaml::require_integer<uint16_t>(node, "max_sessions")),
    listen(yaml::require(node, "listen")),
    connect(yaml::require(node, "connect"))
{

}
