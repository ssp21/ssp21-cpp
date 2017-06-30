#ifndef SSP21PROXY_CONFIGSECTION_H
#define SSP21PROXY_CONFIGSECTION_H

#include "ProxyConfig.h"
#include "ssp21/crypto/gen/ContainerEntryType.h"

#include <map>

class ConfigSection : openpal::Uncopyable
{

public:

    ConfigSection(const std::string& id);

	void add(const std::string& key, const std::string& value);

    std::unique_ptr<ProxyConfig> get_config() const;
	

private:

	const std::string id;

	std::map<std::string, std::string> values;

	std::shared_ptr<ssp21::ICertificateHandler> get_certificate_handler() const;

	openpal::LogLevels get_levels() const;

	std::string get_value(const std::string& key) const;

	template <class T>
	T get_integer_value(const std::string& key) const;

	ProxyConfig::Mode get_mode() const;

	ProxyConfig::CertificateMode get_cert_mode() const;	

	std::shared_ptr<ssp21::SecureDynamicBuffer> get_file_data(const std::string& key) const;

	template <class T>
	std::shared_ptr<const T> get_crypto_key(const std::string& key, ssp21::ContainerEntryType expectedType) const;

	openpal::LogLevels ConfigSection::get_levels_for_char(char value) const;


};

#endif

