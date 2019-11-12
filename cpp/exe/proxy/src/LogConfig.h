#ifndef SSP21PROXY_LOGCONFIG_H
#define SSP21PROXY_LOGCONFIG_H

#include "log4cpp/LogLevels.h"

#include <yaml-cpp/yaml.h>

class LogConfig
{
public:   

    LogConfig(const YAML::Node& node);
    
    const std::string id;
    const log4cpp::LogLevels levels;
};

#endif

