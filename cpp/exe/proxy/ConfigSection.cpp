
#include "ConfigSection.h"

#include "ConfigProperties.h"

#include "ssp21/crypto/ICertificateHandler.h"

#include "ssp21/util/SecureFile.h"
#include "ssp21/util/Exception.h"
#include "ssp21/stack/LogLevels.h"
#include "ssp21/stack/Factory.h"
#include "ssp21/crypto/gen/ContainerFile.h"
#include "ssp21/link/Addresses.h"

#include "ssp21/util/Exception.h"
#include "ssp21/stack/LogLevels.h"
#include "ssp21/crypto/gen/ContainerFile.h"

#include <iostream>


using namespace openpal;
using namespace ssp21;

void ConfigSection::add(const std::string& propertyId, const std::string& value)
{
    this->values[propertyId] = value;
}

std::unique_ptr<ProxyConfig> ConfigSection::get_config(const std::string& id)
{
    const auto endpoint_mode = this->get_mode();
    const openpal::LogLevels levels(this->get_levels());

    auto ret = std::make_unique<ProxyConfig>(
                   id,
                   levels,
                   endpoint_mode,
                   this->get_integer_value<uint16_t>(props::max_sessions),
                   this->get_integer_value<uint16_t>(props::listen_port),
                   this->consume_value(props::listen_endpoint),
                   this->get_integer_value<uint16_t>(props::connect_port),
                   this->consume_value(props::connect_endpoint)
               );

    for (auto& value : this->values)
    {
        std::cerr << "warning: " << "unused value: " << value.first << "=" << value.second << std::endl;
    }

    return std::move(ret);
}

stack_factory_t ConfigSection::get_stack_factory(ProxyConfig::EndpointMode ep_mode)
{
    const Addresses addresses(this->get_addresses());
    return (ep_mode == ProxyConfig::EndpointMode::initiator) ? this->get_initiator_factory(addresses) : this->get_responder_factory(addresses);
}

stack_factory_t ConfigSection::get_initiator_factory(const ssp21::Addresses& addresses)
{
    switch (this->get_cert_mode())
    {
    case(CertificateMode::shared_secert):
        return this->get_initiator_shared_secert_factory(addresses);
    case(CertificateMode::preshared_keys):
        return this->get_initiator_preshared_public_key_factory(addresses);
    case(CertificateMode::certificates):
        return this->get_initiator_certificate_mode_factory(addresses);
    default:
        throw Exception("undefined certificate mode");
    }
}

stack_factory_t ConfigSection::get_initiator_shared_secert_factory(const ssp21::Addresses& addresses)
{
    const auto shared_secret = this->get_shared_secert();

    return [ = ](const openpal::Logger & logger, const std::shared_ptr<openpal::IExecutor>& executor)
    {
        return initiator::factory::shared_secert_mode(
                   addresses,
                   InitiatorConfig(),	// TODO: default
                   logger,
                   executor,
                   CryptoSuite(),		// TODO: default
                   shared_secret
               );
    };
}

stack_factory_t ConfigSection::get_initiator_preshared_public_key_factory(const ssp21::Addresses& addresses)
{
    const auto local_keys = this->get_local_static_keys();
    const auto remote_public_key = this->get_crypto_key<ssp21::PublicKey>(props::remote_public_key_path, ContainerEntryType::x25519_public_key);

    return [ = ](const openpal::Logger & logger, const std::shared_ptr<openpal::IExecutor>& executor)
    {
        return initiator::factory::preshared_public_key_mode(
                   addresses,
                   InitiatorConfig(),	// TODO: default
                   logger,
                   executor,
                   CryptoSuite(),		// TODO: default
                   local_keys,
                   remote_public_key);
    };
}

stack_factory_t ConfigSection::get_initiator_certificate_mode_factory(const ssp21::Addresses& addresses)
{
    const auto local_keys = this->get_local_static_keys();
    const auto anchor_cert_data = this->get_file_data(props::authority_cert_path);
    const auto local_cert_data = this->get_file_data(props::local_cert_path);

    return [ = ](const openpal::Logger & logger, const std::shared_ptr<openpal::IExecutor>& executor)
    {
        return initiator::factory::certificate_public_key_mode(
                   addresses,
                   InitiatorConfig(),	// TODO: default
                   logger,
                   executor,
                   CryptoSuite(),		// TODO: default
                   local_keys,
                   anchor_cert_data,
                   local_cert_data
               );
    };
}

stack_factory_t ConfigSection::get_responder_factory(const ssp21::Addresses& addresses)
{
    switch (this->get_cert_mode())
    {
    case(CertificateMode::shared_secert):
        return this->get_responder_shared_secert_factory(addresses);
    case(CertificateMode::preshared_keys):
        return this->get_responder_preshared_public_key_factory(addresses);
    case(CertificateMode::certificates):
        return this->get_responder_certificate_mode_factory(addresses);
    default:
        throw Exception("undefined certificate mode");
    }
}

stack_factory_t ConfigSection::get_responder_shared_secert_factory(const ssp21::Addresses& addresses)
{
    const auto shared_secret = this->get_shared_secert();

    return [ = ](const openpal::Logger & logger, const std::shared_ptr<openpal::IExecutor>& executor)
    {
        return responder::factory::shared_secret_mode(
                   addresses,
                   ResponderConfig(),	// TODO: default
                   logger,
                   executor,
                   shared_secret
               );
    };
}

stack_factory_t ConfigSection::get_responder_preshared_public_key_factory(const ssp21::Addresses& addresses)
{
    const auto local_keys = this->get_local_static_keys();
    const auto remote_public_key = this->get_crypto_key<ssp21::PublicKey>(props::remote_public_key_path, ContainerEntryType::x25519_public_key);

    return [ = ](const openpal::Logger & logger, const std::shared_ptr<openpal::IExecutor>& executor)
    {
        return responder::factory::preshared_public_key_mode(
                   addresses,
                   ResponderConfig(),	// TODO: default
                   logger,
                   executor,
                   local_keys,
                   remote_public_key);
    };
}

stack_factory_t ConfigSection::get_responder_certificate_mode_factory(const ssp21::Addresses& addresses)
{
    const auto local_keys = this->get_local_static_keys();
    const auto anchor_cert_data = this->get_file_data(props::authority_cert_path);
    const auto local_cert_data = this->get_file_data(props::local_cert_path);

    return [ = ](const openpal::Logger & logger, const std::shared_ptr<openpal::IExecutor>& executor)
    {
        return responder::factory::certificate_public_key_mode(
                   addresses,
                   ResponderConfig(),	// TODO: default
                   logger,
                   executor,
                   local_keys,
                   anchor_cert_data,
                   local_cert_data
               );
    };
}

Addresses ConfigSection::get_addresses()
{
    return Addresses(
               this->get_integer_value<uint16_t>(props::remote_address),
               this->get_integer_value<uint16_t>(props::local_address)
           );
}

ssp21::StaticKeys ConfigSection::get_local_static_keys()
{
    return ssp21::StaticKeys(
               this->get_crypto_key<PublicKey>(props::local_public_key_path, ContainerEntryType::x25519_public_key),
               this->get_crypto_key<PrivateKey>(props::local_private_key_path, ContainerEntryType::x25519_private_key)
           );
}

std::shared_ptr<const SymmetricKey> ConfigSection::get_shared_secert()
{
    return this->get_crypto_key<SymmetricKey>(props::shared_secret_key_path, ContainerEntryType::shared_secret);
}

LogLevels ConfigSection::get_levels()
{
    LogLevels levels;
    for (auto flag : this->consume_value(props::log_levels))
    {
        levels |= this->get_levels_for_char(flag);
    }
    return levels;
}

std::string ConfigSection::consume_value(const std::string& propertyId)
{
    const auto iter = this->values.find(propertyId);
    if (iter == values.end())
    {
        throw Exception("Required property not found: ", propertyId);
    }
    const auto ret = iter->second;
    this->values.erase(iter);
    return ret;
}

template <class T>
T ConfigSection::get_integer_value(const std::string& propertyId)
{
    const auto value = this->consume_value(propertyId);
    std::istringstream reader(value);
    T val = 0;
    if (!(reader >> val))
    {
        throw Exception("bad integer value: ", value);
    }
    return val;
}

ProxyConfig::EndpointMode ConfigSection::get_mode()
{
    const auto value = this->consume_value(props::mode);

    if (value == "initiator")
    {
        return ProxyConfig::EndpointMode::initiator;
    }
    else if (value == "responder")
    {
        return ProxyConfig::EndpointMode::responder;
    }
    else
    {
        throw Exception("Unknown mode: ", value);
    }
}

ConfigSection::CertificateMode ConfigSection::get_cert_mode()
{
    const auto value = this->consume_value(props::certificate_mode);

    if (value == "preshared")
    {
        return CertificateMode::preshared_keys;
    }
    else if (value == "certificate")
    {
        return CertificateMode::certificates;
    }
    else
    {
        throw Exception("Unknown certificate mode: ", value);
    }
}

std::shared_ptr<ssp21::SecureDynamicBuffer> ConfigSection::get_file_data(const std::string& key)
{
    return ssp21::SecureFile::read(this->consume_value(key));
}

template <class T>
std::shared_ptr<const T> ConfigSection::get_crypto_key(const std::string& key, ssp21::ContainerEntryType expectedType)
{
    const auto path = this->consume_value(key);
    const auto file_data = SecureFile::read(path);

    ContainerFile file;
    const auto err = file.read_all(file_data->as_rslice());
    if (any(err))
    {
        throw Exception("Error (", ParseErrorSpec::to_string(err), ")  reading container file: ", path);
    }

    if (file.container_entry_type != expectedType)
    {
        throw Exception("Unexpected file type (", ContainerEntryTypeSpec::to_string(file.container_entry_type), ") in file: ", path);
    }

    const auto expected_length = ssp21::BufferBase::get_buffer_length(ssp21::BufferType::x25519_key);
    if (file.payload.length() != expected_length)
    {
        throw Exception("Unexpected key length: ", file.payload.length());
    }

    const auto ret = std::make_shared<T>();
    ret->as_wseq().copy_from(file.payload);
    ret->set_type(BufferType::x25519_key);

    return ret;
}

LogLevels ConfigSection::get_levels_for_char(char value)
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
        throw ssp21::Exception("unknown log level: ", value);
    }
}


