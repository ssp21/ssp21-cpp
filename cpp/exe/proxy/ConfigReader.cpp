
#include "ConfigReader.h"

#include "inih\ini.h"

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

LogLevels ConfigReader::Section::get_levels()
{
    LogLevels levels;
    for (auto flag : this->log_levels.get(this->id))
    {
        levels |= this->get_levels_for_char(flag);
    }
    return levels;
}

LogLevels ConfigReader::Section::get_levels_for_char(char value)
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
        auto& s = pair.second;

        auto section = std::make_unique<ProxyConfig>(
                           s->id,
                           s->get_levels(),
                           s->mode.get(s->id),
                           ProxyConfig::SSP21(
                               s->local_address.get(s->id),
                               s->remote_address.get(s->id),
                               s->local_public_key.get(s->id),
                               s->local_private_key.get(s->id),
                               s->remote_public_key.get(s->id)
                           ),
                           s->max_sessions.get(s->id),
                           s->listen_port.get(s->id),
                           s->listen_endpoint.get(s->id),
                           s->connect_port.get(s->id),
                           s->connect_endpoint.get(s->id)
                       );

        ret.push_back(std::move(section));
    }

    return ret;
}

void ConfigReader::handle(const std::string& section_name, const std::string& key, const std::string& value)
{
    auto& section = this->get_or_create_section(section_name);

    if (key == section.log_levels.key_name)
    {
        section.log_levels.set(value, section_name);
    }
    else if (key == section.mode.key_name)
    {
        handle_mode(section, value);
    }
    else if (key == section.local_public_key.key_name)
    {
        section.local_public_key.move(read_key<ssp21::PublicKey>(section_name, value, ssp21::BufferType::x25519_key), section_name);
    }
    else if (key == section.local_private_key.key_name)
    {
        section.local_private_key.move(read_key<ssp21::PrivateKey>(section_name, value, ssp21::BufferType::x25519_key), section_name);
    }
    else if (key == section.remote_public_key.key_name)
    {
        section.remote_public_key.move(read_key<ssp21::PublicKey>(section_name, value, ssp21::BufferType::x25519_key), section_name);
    }
    else if (key == section.local_address.key_name)
    {
        section.local_address.set(read_integer<uint16_t>(section_name, value), section_name);
    }
    else if (key == section.remote_address.key_name)
    {
        section.remote_address.set(read_integer<uint16_t>(section_name, value), section_name);
    }
    else if (key == section.max_sessions.key_name)
    {
        section.max_sessions.set(read_integer<uint16_t>(section_name, value), section_name);
    }
    else if (key == section.listen_endpoint.key_name)
    {
        section.listen_endpoint.set(value, section_name);
    }
    else if (key == section.listen_port.key_name)
    {
        section.listen_port.set(read_integer<uint16_t>(section_name, value), section_name);
    }
    else if (key == section.connect_endpoint.key_name)
    {
        section.connect_endpoint.set(value, section_name);
    }
    else if (key == section.connect_port.key_name)
    {
        section.connect_port.set(read_integer<uint16_t>(section_name, value), section_name);
    }
    else
    {
        THROW_LOGIC_ERR("unknown key: " << key, section_name);
    }
}

void ConfigReader::handle_mode(Section& section, const std::string& value)
{
    if (value == "initiator")
    {
        section.mode.set(ProxyConfig::Mode::initiator, section.id);
    }
    else if (value == "responder")
    {
        section.mode.set(ProxyConfig::Mode::responder, section.id);
    }
    else
    {
        THROW_LOGIC_ERR("Unknown mode: " << value, section.id);
    }
}

template <class T>
std::shared_ptr<const T> ConfigReader::read_key(const std::string& section, const std::string& value, ssp21::BufferType type)
{
    const auto expected_size = ssp21::BufferBase::get_buffer_length(type);
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

    key->set_type(type);

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

ConfigReader::Section& ConfigReader::get_or_create_section(const std::string& section)
{
    const auto iter = this->sections.find(section);

    if (iter == this->sections.end())
    {
        auto ptr = std::make_unique<Section>(section);
        auto& ret = *ptr;
        this->sections[section] = std::move(ptr);
        return ret;
    }
    else
    {
        return *iter->second;
    }
}