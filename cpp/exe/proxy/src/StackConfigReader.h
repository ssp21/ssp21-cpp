#ifndef SSP21PROXY_STACKCONFIGREADER_H
#define SSP21PROXY_STACKCONFIGREADER_H

#include "StackFactory.h"

#include <yaml-cpp/yaml.h>

namespace config
{  
    StackFactory get_stack_factory(const YAML::Node& node);       
}

#endif

