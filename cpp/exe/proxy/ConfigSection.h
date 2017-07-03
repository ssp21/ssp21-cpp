#ifndef SSP21PROXY_CONFIGSECTION_H
#define SSP21PROXY_CONFIGSECTION_H

#include "ProxyConfig.h"
#include "ssp21/crypto/gen/ContainerEntryType.h"

#include <map>

class ConfigSection : openpal::Uncopyable
{

public:    

	void add(const std::string& propertyId, const std::string& value);

	std::unique_ptr<ProxyConfig> get_config(const std::string& id);
	

private:
	
	std::map<std::string, std::string> values;

	std::shared_ptr<ssp21::ICertificateHandler> get_certificate_handler();

	openpal::LogLevels get_levels();

	std::string consume_value(const std::string& propertyId);

	template <class T>
	T get_integer_value(const std::string& propertyId);

	ProxyConfig::Mode get_mode();

	ProxyConfig::CertificateMode get_cert_mode();	

	std::shared_ptr<ssp21::SecureDynamicBuffer> get_file_data(const std::string& property);

	template <class T>
	std::shared_ptr<const T> get_crypto_key(const std::string& key, ssp21::ContainerEntryType expectedType);

	static openpal::LogLevels get_levels_for_char(char value);


};

#endif

