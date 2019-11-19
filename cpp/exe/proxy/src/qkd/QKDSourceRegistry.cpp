#include "QKDSourceRegistry.h"

#include <ssp21/util/Exception.h>

#include "QIXQKDSource.h"
#include "YAMLHelpers.h"

std::map<std::string, std::shared_ptr<IQKDSource>> QKDSourceRegistry::sources;

void QKDSourceRegistry::configure_qkd_source(const YAML::Node& node, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, log4cpp::Logger logger)
{	
	const auto qkd_source_id = yaml::require_string(node, "qkd_source_id");

	if (sources.find(qkd_source_id) != sources.end()) {
		throw yaml::YAMLException(node.Mark(), "Duplicate QKD source id: ", qkd_source_id);
	}

	sources[qkd_source_id] = create_qkd_source(node, executor, logger);
}

std::shared_ptr<IQKDSource> QKDSourceRegistry::get_key_source(const YAML::Node& node)
{
	const auto key_source_id = yaml::require_string(node, "key_source_id");
	const auto iter = sources.find(key_source_id);
	if (iter == sources.end()) {
		throw yaml::YAMLException(node.Mark(), "Unknown key source id: ", key_source_id);
	}
	return iter->second;
}

std::shared_ptr<IQKDSource> QKDSourceRegistry::create_qkd_source(const YAML::Node& node, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, log4cpp::Logger logger)
{
	const auto type = yaml::require_string(node, "type");

	if (type == "qix")
	{
		return std::make_shared<QIXQKDSource>(node, executor, logger);
	}

	throw yaml::YAMLException(node.Mark(), "Unknown QKD source type: ", type);
}

uint16_t get_subscriber_id(const YAML::Node& node)
{
	return yaml::require_integer<uint16_t>(node, "subscriber_id");
}

uint16_t get_max_key_cache_size(const YAML::Node& node)
{
	return yaml::require_integer<uint16_t>(node, "max_key_cache_size");
}

std::shared_ptr<ssp21::IKeySource> QKDSourceRegistry::get_initiator_key_source(const YAML::Node& node)
{						
	return get_key_source(node)->bind_initiator_key_source(get_subscriber_id(node), get_max_key_cache_size(node));
}

std::shared_ptr<ssp21::IKeyLookup> QKDSourceRegistry::get_responder_key_lookup(const YAML::Node& node)
{
	return get_key_source(node)->bind_responder_key_lookup(get_subscriber_id(node), get_max_key_cache_size(node));
}



//static std::map<std::string, std::unique_ptr<IQKDSource>> sources;
