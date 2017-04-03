
#include "ConfigSection.h"

#include "ssp21/stack/LogLevels.h"

using namespace openpal;

ConfigSection::ConfigSection(const std::string& id) :
    id(id),
    log_levels("log_levels"),
    mode("mode"),
    local_public_key("local_public_key"),
    local_private_key("local_private_key"),
    remote_public_key("remote_public_key"),
    local_address("local_address"),
    remote_address("remote_address"),
    max_sessions("max_sessions"),
    listen_port("listen_port"),
    listen_endpoint("listen_endpoint"),
    connect_port("connect_port"),
    connect_endpoint("connect_endpoint")
{}

std::unique_ptr<ProxyConfig> ConfigSection::get_config() const
{
    return std::make_unique<ProxyConfig>(
               this->id,
               this->get_levels(),
               this->mode.get(this->id),
               ProxyConfig::SSP21(
                   this->local_address.get(this->id),
                   this->remote_address.get(this->id),
                   this->local_public_key.get(this->id),
                   this->local_private_key.get(this->id),
                   this->remote_public_key.get(this->id)
               ),
               this->max_sessions.get(this->id),
               this->listen_port.get(this->id),
               this->listen_endpoint.get(this->id),
               this->connect_port.get(this->id),
               this->connect_endpoint.get(this->id)
           );
}

LogLevels ConfigSection::get_levels() const
{
    LogLevels levels;
    for (auto flag : this->log_levels.get(this->id))
    {
        levels |= this->get_levels_for_char(flag);
    }
    return levels;
}

LogLevels ConfigSection::get_levels_for_char(char value) const
{
    switch (value)
    {
    case('v'):
        return LogLevels(ssp21::levels::event.value);
    case('e'):
        return LogLevels(ssp21::levels::error.value);
    case('w'):
        return LogLevels(ssp21::levels::warn.value);
    case('i'):
        return LogLevels(ssp21::levels::info.value);
    case('d'):
        return LogLevels(ssp21::levels::debug.value);
    case('m'):
        return LogLevels(ssp21::levels::rx_crypto_msg.value | ssp21::levels::tx_crypto_msg.value);
    case('f'):
        return LogLevels(ssp21::levels::rx_crypto_msg_fields.value | ssp21::levels::tx_crypto_msg_fields.value);
    default:
        THROW_LOGIC_ERR("unknown log level: " << value, this->id);
    }
}


