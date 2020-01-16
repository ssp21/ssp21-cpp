#ifndef SSP21PROXY_CONFIGREADER_H
#define SSP21PROXY_CONFIGREADER_H

#include "ProxySessionFactory.h"

#include <yaml-cpp/yaml.h>

namespace config {
proxy_session_factory_t get_session_factory(const YAML::Node& node);
}

#endif
