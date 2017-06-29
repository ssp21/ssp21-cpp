#ifndef SSP21PROXY_CONFIGREADER_H
#define SSP21PROXY_CONFIGREADER_H

#include "ConfigSection.h"

#include <map>
#include <vector>
#include <functional>

#include "ssp21/crypto/gen/ContainerEntryType.h"

class ConfigReader
{
    typedef std::function<void(ConfigSection& section, const std::string& value)> key_handler_func_t;

public:

    static std::vector<std::unique_ptr<ProxyConfig>> read(const std::string& file_path);

private:

    std::map<std::string, key_handler_func_t> key_handler_map;

    std::map<std::string, std::unique_ptr<ConfigSection>> sections;

    ConfigReader();

    static int config_ini_handler(void* user, const char* section, const char* key, const char* value);

    void handle(const std::string& section_name, const std::string& key, const std::string& value);

    ConfigSection& get_or_create_section(const std::string& section);

    // ----- static helpers for reading various types -----

    static ProxyConfig::Mode read_mode(const std::string& section, const std::string& value);

	static ProxyConfig::CertificateMode read_cert_mode(const std::string& section, const std::string& value);

    template <class T>
    static std::shared_ptr<const T> read_key_from_file(const std::string& section, const std::string& path, ssp21::ContainerEntryType expectedTyp);	

    template <class T>
    static T read_integer(const std::string& section, const std::string& value);


};

#endif

