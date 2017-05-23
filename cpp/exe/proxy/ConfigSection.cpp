
#include "ConfigSection.h"

#include "ssp21/stack/LogLevels.h"

#include "ConfigKeys.h"

using namespace openpal;

ConfigSection::ConfigSection(const std::string& id) :
    id(id),
    log_levels(keys::log_levels),
    mode(keys::mode),
    local_public_key(keys::local_public_key_path),
    local_private_key(keys::local_private_key_path),
    remote_public_key(keys::remote_public_key_path),
    local_address(keys::local_address),
    remote_address(keys::remote_address),
    max_sessions(keys::max_sessions),
    listen_port(keys::listen_port),
    listen_endpoint(keys::listen_endpoint),
    connect_port(keys::connect_port),
    connect_endpoint(keys::connect_endpoint)
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
        throw SectionException(this->id, "unknown log level: ", value);
    }
}


