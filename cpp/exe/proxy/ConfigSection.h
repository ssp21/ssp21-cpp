#ifndef SSP21PROXY_CONFIGSECTION_H
#define SSP21PROXY_CONFIGSECTION_H

#include "ProxyConfig.h"

#include "ssp21/crypto/gen/ContainerEntryType.h"
#include "ssp21/util/SecureDynamicBuffer.h"
#include "ssp21/stack/IStack.h"
#include "ssp21/link/Addresses.h"
#include "ssp21/crypto/StaticKeys.h"

#include "StackFactory.h"

#include <map>
#include <memory>

class ConfigSection : openpal::Uncopyable
{
    enum class CertificateMode
    {
        preshared_keys,
        certificates
    };

public:

    void add(const std::string& propertyId, const std::string& value);

    std::unique_ptr<ProxyConfig> get_config(const std::string& id);

private:

    std::map<std::string, std::string> values;

    stack_factory_t get_stack_factory(ProxyConfig::EndpointMode ep_mode);

    stack_factory_t get_initiator_factory(const ssp21::Addresses& addresses);

    stack_factory_t get_initiator_preshared_public_key_factory(const ssp21::Addresses& addresses);

    stack_factory_t get_initiator_certificate_mode_factory(const ssp21::Addresses& addresses);

    stack_factory_t get_responder_factory(const ssp21::Addresses& addresses);

    stack_factory_t get_responder_preshared_public_key_factory(const ssp21::Addresses& addresses);

    stack_factory_t get_responder_certificate_mode_factory(const ssp21::Addresses& addresses);

    ssp21::Addresses get_addresses();

    ssp21::StaticKeys get_local_static_keys();

    openpal::LogLevels get_levels();

    std::string consume_value(const std::string& propertyId);

    template <class T>
    T get_integer_value(const std::string& propertyId);

    ProxyConfig::EndpointMode get_mode();

    CertificateMode get_cert_mode();

    std::shared_ptr<ssp21::SecureDynamicBuffer> get_file_data(const std::string& property);

    template <class T>
    std::shared_ptr<const T> get_crypto_key(const std::string& key, ssp21::ContainerEntryType expectedType);

    static openpal::LogLevels get_levels_for_char(char value);


};

#endif

