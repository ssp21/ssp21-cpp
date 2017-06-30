#ifndef SSP21PROXY_CONFIGREADER_H
#define SSP21PROXY_CONFIGREADER_H

#include "ConfigSection.h"

#include <vector>
#include <map>

class ConfigReader
{
    
public:

    static std::vector<std::unique_ptr<ProxyConfig>> read(const std::string& file_path);

private:    

    std::map<std::string, std::unique_ptr<ConfigSection>> sections;

	ConfigReader() {}

    static int config_ini_handler(void* user, const char* section, const char* key, const char* value);

    void handle(const std::string& section_name, const std::string& key, const std::string& value);

    ConfigSection& get_or_create_section(const std::string& section);

};

#endif

