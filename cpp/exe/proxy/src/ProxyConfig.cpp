#include "ProxyConfig.h"

#include "YAMLHelpers.h"

#include "ConfigReader.h"

#include "qkd/QKDSourceRegistry.h"

namespace config {

	std::vector<proxy_session_factory_t> read(const std::string& file_path, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, const log4cpp::Logger& logger)
	{
		const YAML::Node root = YAML::LoadFile(file_path);

		yaml::foreach(
			yaml::require(root, "qkd_sources"),
			[&](const YAML::Node& node) {
				QKDSourceRegistry::configure_qkd_source(node, executor, logger);
			}
		);

		std::vector<proxy_session_factory_t> factories;

		yaml::foreach(
			yaml::require(root, "sessions"),
			[&](const YAML::Node& node) {
				factories.push_back(config::get_session_factory(node));
			}
		);

		return factories;
	}

}

