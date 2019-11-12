#include "LogConfig.h"

#include "YAMLHelpers.h"

#include <ssp21/stack/LogLevels.h>


log4cpp::LogLevels get_log_levels(const YAML::Node& node)
{
    log4cpp::LogLevels levels;
    for (auto flag : yaml::require_string(node, "levels"))
    {
        switch (flag)
        {
        case('v'):
            levels |= log4cpp::LogLevels(ssp21::levels::event.value);
            break;
        case('e'):
            levels |= log4cpp::LogLevels(ssp21::levels::error.value);
            break;
        case('w'):
            levels |= log4cpp::LogLevels(ssp21::levels::warn.value);
            break;
        case('i'):
            levels |= log4cpp::LogLevels(ssp21::levels::info.value);
            break;
        case('d'):
            levels |= log4cpp::LogLevels(ssp21::levels::debug.value);
            break;
        case('m'):
            levels |= log4cpp::LogLevels(ssp21::levels::rx_crypto_msg.value | ssp21::levels::tx_crypto_msg.value);
            break;
        case('f'):
            levels |= log4cpp::LogLevels(ssp21::levels::rx_crypto_msg_fields.value | ssp21::levels::tx_crypto_msg_fields.value);
            break;
        default:
            throw yaml::YAMLException(node, "unknown log level: ", flag);
        }
    }
    return levels;
}

LogConfig::LogConfig(const YAML::Node& node) :
    id(yaml::require_string(node, "id")),
    levels(get_log_levels(node))
{

}
