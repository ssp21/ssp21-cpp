
#include "ConfigSection.h"

#include "ConfigKeys.h"

#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/util/SecureFile.h"
#include "ssp21/util/Exception.h"
#include "ssp21/stack/LogLevels.h"
#include "ssp21/crypto/gen/ContainerFile.h"

#include "ssp21/util/Exception.h"
#include "ssp21/stack/LogLevels.h"
#include "ssp21/crypto/gen/ContainerFile.h"

#include <iostream>

using namespace openpal;
using namespace ssp21;

void ConfigSection::add(const std::string& key, const std::string& value)
{
	this->values[key] = value;
}

std::unique_ptr<ProxyConfig> ConfigSection::get_config(const std::string& id)
{
    auto ret = std::make_unique<ProxyConfig>(
               id,
               this->get_levels(),
               this->get_mode(),
               ProxyConfig::SSP21(
                   this->get_integer_value<uint16_t>(keys::local_address),
                   this->get_integer_value<uint16_t>(keys::remote_address),
				   ssp21::StaticKeys(
					   this->get_crypto_key<PublicKey>(keys::local_public_key_path, ContainerEntryType::x25519_public_key),
					   this->get_crypto_key<PrivateKey>(keys::local_private_key_path, ContainerEntryType::x25519_private_key)
				   ),
				   this->get_certificate_handler()
               ),
               this->get_integer_value<uint16_t>(keys::max_sessions),
			   this->get_integer_value<uint16_t>(keys::listen_port),
               this->consume_value(keys::listen_endpoint),
			   this->get_integer_value<uint16_t>(keys::connect_port),
               this->consume_value(keys::connect_endpoint)
           );

	for (auto& value : this->values)
	{
		std::cerr << "warning: " << "unused value: " << value.first << "=" << value.second << std::endl;
	}

	return std::move(ret);
}

std::shared_ptr<ssp21::ICertificateHandler> ConfigSection::get_certificate_handler()
{	
	if (this->get_cert_mode() == ProxyConfig::CertificateMode::preshared_keys)
	{
		return ssp21::ICertificateHandler::preshared_key(
			this->get_crypto_key<ssp21::PublicKey>(
				keys::remote_public_key_path, 
				ContainerEntryType::x25519_public_key			
			)
		);
	}
	else
	{
		return ssp21::ICertificateHandler::certificates(
			this->get_file_data(keys::authority_cert_path),
			this->get_file_data(keys::local_cert_path)
		);
	}		
}

LogLevels ConfigSection::get_levels()
{	
    LogLevels levels;	
    for (auto flag : this->consume_value(keys::log_levels))
    {
        levels |= this->get_levels_for_char(flag);
    }	
    return levels;
}

std::string ConfigSection::consume_value(const std::string& key)
{
	const auto iter = this->values.find(key);
	if (iter == values.end())
	{
		throw Exception("Required key not found: ", key);
	}
	const auto ret = iter->second;
	this->values.erase(iter);
	return ret;
}

template <class T>
T ConfigSection::get_integer_value(const std::string& key)
{
	const auto value = this->consume_value(key);
	std::istringstream reader(value);
	T val = 0;
	if (!(reader >> val))
	{
		throw Exception("bad integer value: ", value);
	}
	return val;
}

ProxyConfig::Mode ConfigSection::get_mode()
{
	const auto value = this->consume_value(keys::mode);

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
		throw Exception("Unknown mode: ", value);
	}
}

ProxyConfig::CertificateMode ConfigSection::get_cert_mode()
{
	const auto value = this->consume_value(keys::certificate_mode);

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


