#ifndef SSP21PROXY_QKDSOURCEREGISTRY_H
#define SSP21PROXY_QKDSOURCEREGISTRY_H

#include "IQKDSource.h"

#include <ssp21/crypto/IKeySource.h>
#include <ssp21/crypto/IKeyLookup.h>

#include <exe4cpp/asio/BasicExecutor.h>


#include <log4cpp/Logger.h>
#include <yaml-cpp/yaml.h>

#include <map>
#include <string>
#include <memory>

/**
* Static methods for configuring and retrieving QKD interfaces during configuration
*  
* No thread synchronization is required on these methods as they are only called during
* initialization from the entry point thread
*/
class QKDSourceRegistry
{
public:

	/**
	* Configure a QKD source from YAML or throw an exception on configuration error
	*/
	static void configure_qkd_source(const YAML::Node& node, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, log4cpp::Logger logger);

	/**
	* Retrieve an interface used to retrieve keys on an initiator
	*/
	static std::shared_ptr<ssp21::IKeySource> get_initiator_key_source(const YAML::Node& node);

	/**
	* Retrieve an interface used to lookup keys on a responder
	*/
	static std::shared_ptr<ssp21::IKeyLookup> get_responder_key_lookup(const YAML::Node& node);

private:

	static std::shared_ptr<IQKDSource> get_key_source(const YAML::Node& node);

	static std::shared_ptr<IQKDSource> create_qkd_source(const YAML::Node& node, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, log4cpp::Logger logger);

	static std::map<std::string, std::shared_ptr<IQKDSource>> sources;
};

#endif
