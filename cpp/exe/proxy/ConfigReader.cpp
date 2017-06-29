
#include "ConfigReader.h"

#include "ConfigKeys.h"

#include "inih/ini.h"

#include "ssp21/util/SecureFile.h"
#include "ssp21/stack/LogLevels.h"
#include "ssp21/crypto/gen/ContainerFile.h"

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
        throw std::logic_error("config file not found");
    default:
        throw std::logic_error("error reading config file");
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

	this->key_handler_map[keys::mode] = [](ConfigSection & section, const std::string & value)
	{
		section.certificate_mode.set(read_cert_mode(section.id, value), section.id);
	};

    this->key_handler_map[keys::local_public_key_path] = [](ConfigSection & section, const std::string & path)
    {
		section.local_public_key_path.set(path, section.id);// read_key_from_file<ssp21::PublicKey>(section.id, path, ContainerEntryType::x25519_public_key), section.id);
    };

    this->key_handler_map[keys::local_private_key_path] = [](ConfigSection & section, const std::string & path)
    {
		section.local_private_key_path.set(path, section.id); // read_key_from_file<ssp21::PrivateKey>(section.id, path, ContainerEntryType::x25519_private_key), section.id);
    };

    this->key_handler_map[keys::remote_public_key_path] = [](ConfigSection & section, const std::string & path)
    {
		section.remote_public_key_path.set(path, section.id); // read_key_from_file<ssp21::PublicKey>(section.id, path, ContainerEntryType::x25519_public_key), section.id);
    };

	this->key_handler_map[keys::authority_cert_path] = [](ConfigSection & section, const std::string & path)
	{
		section.authority_cert_path.set(path, section.id);
	};

	this->key_handler_map[keys::local_cert_path] = [](ConfigSection & section, const std::string & path)
	{
		section.local_cert_path.set(path, section.id);
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
        throw SectionException(section_name, "unknown key: ", key);
    }
    else
    {
        iter->second(section, value);
    }
}

ProxyConfig::Mode ConfigReader::read_mode(const std::string& section_name, const std::string& value)
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
        throw SectionException(section_name, "Unknown mode: ", value);
    }
}

ProxyConfig::CertificateMode ConfigReader::read_cert_mode(const std::string& section, const std::string& value)
{
	if (value == "preshared")
	{
		return ProxyConfig::CertificateMode::preshared_keys;
	}
	else if (value == "certificate")
	{
		return ProxyConfig::CertificateMode::certificates;
	}
	else
	{
		throw SectionException(section, "Unknown certificate mode: ", value);
	}
}

template <class T>
T ConfigReader::read_integer(const std::string& section_name, const std::string& value)
{
    std::istringstream reader(value);
    T val = 0;
    if (!(reader >> val))
    {
        throw SectionException(section_name, "bad integer value: ", value);
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
