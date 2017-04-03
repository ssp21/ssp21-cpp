#ifndef SSP21PROXY_CONFIGREADER_H
#define SSP21PROXY_CONFIGREADER_H


#include "ProxyConfig.h"

#include <map>
#include <stdexcept>
#include <vector>
#include <sstream>

#define THROW_LOGIC_ERR(build_pattern, section) { \
    std::ostringstream oss; \
    oss << build_pattern << " [section = " << section << "]"; \
    throw std::logic_error(oss.str()); \
}


class ConfigReader
{

public:

    static std::vector<std::unique_ptr<ProxyConfig>> read(const std::string& file_path);

private:

    ConfigReader() = default;

    template <class T>
    class SetOnce : openpal::Uncopyable
    {
    public:

        SetOnce(const char* key_name) : key_name(key_name)
        {}

        bool is_defined() const
        {
            return defined;
        }

        inline T& get(const std::string& section)
        {
            if (!defined)
            {
                THROW_LOGIC_ERR("value not defined: " << key_name, section);
            }
            return value;
        }

        void set(const T& value, const std::string& section)
        {
            if (defined)
            {
                THROW_LOGIC_ERR("value already defined: " << key_name, section);
            }
            defined = true;
            this->value = value;
        }

        void move(T& value, const std::string& section)
        {
            if (defined)
            {
                THROW_LOGIC_ERR("value already defined: " << key_name, section);
            }
            defined = true;
            this->value = std::move(value);
        }

        const char* const key_name;

    private:

        T value;
        bool defined = false;
    };

    struct Section : openpal::Uncopyable
    {
        Section(const std::string& id) :
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

        openpal::LogLevels get_levels();

        const std::string id;

        SetOnce<std::string> log_levels;
        SetOnce<ProxyConfig::Mode> mode;

        SetOnce<std::shared_ptr<const ssp21::PublicKey>> local_public_key;
        SetOnce<std::shared_ptr<const ssp21::PrivateKey>> local_private_key;
        SetOnce<std::shared_ptr<const ssp21::PublicKey>> remote_public_key;

        SetOnce<uint16_t> local_address;
        SetOnce<uint16_t> remote_address;

        SetOnce<uint16_t> max_sessions;
        SetOnce<uint16_t> listen_port;
        SetOnce<std::string> listen_endpoint;

        SetOnce<uint16_t> connect_port;
        SetOnce<std::string> connect_endpoint;

    private:

        openpal::LogLevels get_levels_for_char(char value);
    };

    static int config_ini_handler(void* user, const char* section, const char* key, const char* value);

    void handle(const std::string& section_name, const std::string& key, const std::string& value);

    static void handle_mode(Section& section, const std::string& value);

    Section& get_or_create_section(const std::string& section);

    template <class T>
    static std::shared_ptr<const T> read_key(const std::string& section, const std::string& value, ssp21::BufferType type);

    template <class T>
    static T read_integer(const std::string& section, const std::string& value);

    std::map<std::string, std::unique_ptr<Section>> sections;
};

#endif

