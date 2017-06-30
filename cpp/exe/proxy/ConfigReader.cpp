
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
    reinterpret_cast<ConfigReader*>(user)->handle(section, key, value);
    return 1;    
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

void ConfigReader::handle(const std::string& section_name, const std::string& key, const std::string& value)
{
    this->get_or_create_section(section_name).add(key, value);
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


