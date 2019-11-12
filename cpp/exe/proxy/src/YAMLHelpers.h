#ifndef SSP21PROXY_YAMLHELPERS_H
#define SSP21PROXY_YAMLHELPERS_H

#include <yaml-cpp/yaml.h>

#include <ssp21/util/Exception.h>
#include <exe4cpp/Typedefs.h>

#include <stdexcept>
#include <sstream>
#include <limits>

namespace yaml
{		
	class YAMLException : public ssp21::Exception
	{
	public:

		template <class T, class... Args>
		YAMLException(T t, Args... args)
			: ssp21::Exception(t, args...)
		{
		}		

		template <class T, class... Args>
		YAMLException(const YAML::Mark& mark, T t, Args... args)
			: ssp21::Exception(t, args..., " line: ", mark.line)
		{
		}
	};

    std::string get_node_type(const YAML::Node& node);

    YAML::Node require(const YAML::Node& node, const std::string& key);

    bool require_bool(const YAML::Node& node, const std::string& key);

    std::string require_string(const YAML::Node& node, const std::string& key);

    std::string optional_string(const YAML::Node& node, const std::string& key, const std::string& default_value);

	exe4cpp::duration_t optional_duration(const YAML::Node& node, const std::string& key, exe4cpp::duration_t default_value);

	template <typename T>
	T extract_integer(const YAML::Node& node)
	{
		static_assert(sizeof(T) <= sizeof(int64_t), "integer size > sizeof(int64_t)");

		const auto value = node.as<int64_t>();

		if (value < std::numeric_limits<T>::min()) {
			throw yaml::YAMLException(node.Mark(), "value is less than the mininum value of ", std::numeric_limits<T>::min());
		}

		if (value > std::numeric_limits<T>::max()) {
			throw yaml::YAMLException(node.Mark(), "value is greater than the maximum value of ", std::numeric_limits<T>::max());
		}

		return static_cast<T>(value);
	}

	template <typename T>
	T require_integer(const YAML::Node& node, const std::string& key)
	{				
		return extract_integer<T>(yaml::require(node, key));
	}

	template <typename T>
	T optional_integer(const YAML::Node& node, const std::string& key, T default_value)
	{				
		if (!node) {
			return default_value;
		}

		const auto child = node[key];

		if (!child) {
			return default_value;
		}
		
		return extract_integer<T>(child);
	}       

	template <class Action>
	void foreach(const YAML::Node& node, const Action& action)
	{
		if (!node) {
			throw YAMLException("empty node argument in foreach()");
		}
			
		if (!node.IsSequence()) {
			throw YAMLException(node.Mark(), "node is not a sequence");
		}
			
		for (auto it = node.begin(); it != node.end(); ++it) {
			action(*it);
		}
	}
}

#endif

