
#include "ConfigReader.h"

#include "ConfigKeys.h"

#include "inih/ini.h"

#include "ssp21/util/Base64.h"
#include "ssp21/stack/LogLevels.h"

#include <iostream>
#include <sstream>

using namespace openpal;
using namespace ssp21;

int ConfigReader::config_ini_handler(void* user, const char* section, const char* key, const char* value)
{
    try
    {
        reinterpret_cast<ConfigReader*>(user)->handle(section, key, value);
        return 1;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 0;
    }
}

std::vector<std::unique_ptr<ProxyConfig>> ConfigReader::read(const std::string& file_path)
{
    ConfigReader c;

    const auto result = ini_parse(file_path.c_str(), config_ini_handler, &c);

    switch (result)
    {
    case(0):
        break;
    case(-1):
        throw std::exception("config file not found");
    default:
        throw std::exception("error reading config file");
    }

    std::vector<std::unique_ptr<ProxyConfig>> ret;

    for (auto& pair : c.sections)
    {
        ret.push_back(pair.second->get_config());
    }

    return ret;
}

ConfigReader::ConfigReader()
{
    this->key_handler_map[keys::log_levels] = [](ConfigSection & section, const std::string & value)
    {
        section.log_levels.set(value, section.id);
    };

    this->key_handler_map[keys::mode] = [](ConfigSection & section, const std::string & value)
    {
        section.mode.set(read_mode(section.id, value), section.id);
    };

    this->key_handler_map[keys::local_public_key] = [](ConfigSection & section, const std::string & value)
    {
        section.local_public_key.move(read_key<ssp21::PublicKey>(section.id, value), section.id);
    };

    this->key_handler_map[keys::local_private_key] = [](ConfigSection & section, const std::string & value)
    {
        section.local_private_key.move(read_key<ssp21::PrivateKey>(section.id, value), section.id);
    };

    this->key_handler_map[keys::remote_public_key] = [](ConfigSection & section, const std::string & value)
    {
        section.remote_public_key.move(read_key<ssp21::PublicKey>(section.id, value), section.id);
    };

    this->key_handler_map[keys::local_address] = [](ConfigSection & section, const std::string & value)
    {
        section.local_address.set(read_integer<uint16_t>(section.id, value), section.id);
    };

    this->key_handler_map[keys::remote_address] = [](ConfigSection & section, const std::string & value)
    {
        section.remote_address.set(read_integer<uint16_t>(section.id, value), section.id);
    };

    this->key_handler_map[keys::max_sessions] = [](ConfigSection & section, const std::string & value)
    {
        section.max_sessions.set(read_integer<uint16_t>(section.id, value), section.id);
    };

    this->key_handler_map[keys::listen_endpoint] = [](ConfigSection & section, const std::string & value)
    {
        section.listen_endpoint.set(value, section.id);
    };

    this->key_handler_map[keys::listen_port] = [](ConfigSection & section, const std::string & value)
    {
        section.listen_port.set(read_integer<uint16_t>(section.id, value), section.id);
    };

    this->key_handler_map[keys::connect_endpoint] = [](ConfigSection & section, const std::string & value)
    {
        section.connect_endpoint.set(value, section.id);
    };

    this->key_handler_map[keys::connect_port] = [](ConfigSection & section, const std::string & value)
    {
        section.connect_port.set(read_integer<uint16_t>(section.id, value), section.id);
    };
}

void ConfigReader::handle(const std::string& section_name, const std::string& key, const std::string& value)
{
    auto& section = this->get_or_create_section(section_name);

    auto iter = this->key_handler_map.find(key);
    if(iter == this->key_handler_map.end())
    {
        THROW_LOGIC_ERR("unknown key: " << key, section_name);
    }
    else
    {
        iter->second(section, value);
    }
}

ProxyConfig::Mode ConfigReader::read_mode(const std::string& section, const std::string& value)
{
    if (value == "initiator")
    {
        return ProxyConfig::Mode::initiator;
    }
    else if (value == "responder")
    {
        return ProxyConfig::Mode::responder;
    }
    else
    {
        THROW_LOGIC_ERR("Unknown mode: " << value, section);
    }
}

template <class T>
std::shared_ptr<const T> ConfigReader::read_key(const std::string& section, const std::string& value)
{
    const auto expected_size = ssp21::BufferBase::get_buffer_length(ssp21::BufferType::x25519_key);
    size_t num_bytes = 0;
    auto key = std::make_shared<T>();
    auto wseq = key->as_wseq();

    const auto byte_writer = [&](uint8_t byte)
    {
        if (!wseq.put(byte))
        {
            THROW_LOGIC_ERR("too many key bytes: " << num_bytes, section);
        }

        ++num_bytes;
    };

    static_assert(sizeof(char) == sizeof(uint8_t), "bad char size");
    const ssp21::seq32_t characters(reinterpret_cast<const uint8_t*>(value.c_str()), value.length());

    const auto result = ssp21::Base64::decode(characters, byte_writer);

    if (any(result))
    {
        THROW_LOGIC_ERR("base64 decode error: " << ssp21::Base64DecodeErrorSpec::to_string(result), section);
    }

    if (num_bytes != expected_size)
    {
        THROW_LOGIC_ERR("bad key length: " << num_bytes, section);
    }

    key->set_type(ssp21::BufferType::x25519_key);

    return key;
}

template <class T>
T ConfigReader::read_integer(const std::string& section, const std::string& value)
{
    std::istringstream reader(value);
    T val = 0;
    if (!(reader >> val))
    {
        THROW_LOGIC_ERR("bad integer value: " << value, section);
    }
    return val;
}

ConfigSection& ConfigReader::get_or_create_section(const std::string& section)
{
    const auto iter = this->sections.find(section);

    if (iter == this->sections.end())
    {
        auto ptr = std::make_unique<ConfigSection>(section);
        auto& ret = *ptr;
        this->sections[section] = std::move(ptr);
        return ret;
    }
    else
    {
        return *iter->second;
    }
}