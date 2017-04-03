#ifndef SSP21PROXY_CONFIGSECTION_H
#define SSP21PROXY_CONFIGSECTION_H

#include "ConfigField.h"
#include "ProxyConfig.h"

class ConfigSection : openpal::Uncopyable
{

public:

    ConfigSection(const std::string& id);

    std::unique_ptr<ProxyConfig> get_config() const;

    const std::string id;

    ConfigField<std::string> log_levels;
    ConfigField<ProxyConfig::Mode> mode;

    ConfigField<std::shared_ptr<const ssp21::PublicKey>> local_public_key;
    ConfigField<std::shared_ptr<const ssp21::PrivateKey>> local_private_key;
    ConfigField<std::shared_ptr<const ssp21::PublicKey>> remote_public_key;

    ConfigField<uint16_t> local_address;
    ConfigField<uint16_t> remote_address;

    ConfigField<uint16_t> max_sessions;
    ConfigField<uint16_t> listen_port;
    ConfigField<std::string> listen_endpoint;

    ConfigField<uint16_t> connect_port;
    ConfigField<std::string> connect_endpoint;

private:

    openpal::LogLevels get_levels() const;

    openpal::LogLevels get_levels_for_char(char value) const;
};

#endif

