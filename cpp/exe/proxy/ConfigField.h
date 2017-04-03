#ifndef SSP21PROXY_CONFIGFIELD_H
#define SSP21PROXY_CONFIGFIELD_H

#include "ExceptionMacros.h"

#include "openpal/util/Uncopyable.h"

template <class T>
class ConfigField : openpal::Uncopyable
{
public:

    ConfigField(const char* key_name) : key_name(key_name)
    {}

    bool is_defined() const
    {
        return defined;
    }

    inline const T& get(const std::string& section) const
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

#endif

