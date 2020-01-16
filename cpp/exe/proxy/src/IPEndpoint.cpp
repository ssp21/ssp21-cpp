#include "IPEndpoint.h"

#include "YAMLHelpers.h"

IPEndpoint::IPEndpoint(const YAML::Node& node)
    : ip_address(yaml::require_string(node, "address"))
    , port(yaml::require_integer<uint16_t>(node, "port"))
{
}
