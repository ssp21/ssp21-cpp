#include "YAMLHelpers.h"

namespace yaml
{			
    std::string get_node_type(const YAML::Node& node)
    {
        switch (node.Type())
        {
        case(YAML::NodeType::value::Map):
            return "Map";
        case(YAML::NodeType::value::Null):
            return "Null";
        case(YAML::NodeType::value::Scalar):
            return "Scalar";
        case(YAML::NodeType::value::Sequence):
            return "Sequence";
        default:
            return "Undefined";
        }
    }

	enum class TimeUnit
	{
		milliseconds,
		seconds,
		minutes,
		hours
	};

	TimeUnit get_time_unit(const YAML::Node& node)
	{
		const auto value = yaml::require_string(node, "unit");
		
		if (value == "milliseconds") {
			return TimeUnit::milliseconds;
		}

		if (value == "seconds") {
			return TimeUnit::seconds;
		}

		if (value == "minutes") {
			return TimeUnit::minutes;
		}

		if (value == "hours") {
			return TimeUnit::hours;
		}

		throw yaml::YAMLException(node.Mark(), "Unknown time unit: ", value);
	}

	YAML::Node require(const YAML::Node& node, const std::string& key)
	{
		if (!node) {
			throw YAMLException("empty node argument in require_key()");
		}

		if (!node.IsMap()) {			
			throw YAMLException(node.Mark(), "node is not a map (key == ", key, ")");
		}

		const auto child = node[key];

		if (!child) {
			throw YAMLException(node.Mark(), "parent node missing required key: ", key);
		}

		return child;
	}

	bool require_bool(const YAML::Node& node, const std::string& key)
	{
		return require(node, key).as<bool>();
	}

	std::string require_string(const YAML::Node& node, const std::string& key)
	{
		const auto subnode = require(node, key);

		if (!subnode.IsScalar()) {
			throw yaml::YAMLException(subnode.Mark(), "expected Scalar but type is ", get_node_type(subnode));
		}

		return subnode.Scalar();
	}

    std::string optional_string(const YAML::Node& node, const std::string& key, const std::string& default_value)
    {
        if (!node) {
            return default_value;
        }

        const auto value = node[key];

        if (!value) {
            return default_value;
        }


        if (!value.IsScalar()) {
            throw yaml::YAMLException(value.Mark(), "expected Scalar but type is ", get_node_type(value));
        }

        return value.Scalar();
    }

	exe4cpp::duration_t optional_duration(const YAML::Node& node, const std::string& key, exe4cpp::duration_t default_value)
	{
		if (!node) {
			return default_value;
		}

		const auto duration = node[key];

		if (!duration) {
			return default_value;
		}

		const auto value = extract_integer<int64_t>(yaml::require(duration, "value"));

		switch (get_time_unit(duration))
		{
		    case(TimeUnit::milliseconds):
				return std::chrono::milliseconds(value);
			case(TimeUnit::seconds):
				return std::chrono::seconds(value);
			case(TimeUnit::minutes):
				return std::chrono::minutes(value);
			default:
				return std::chrono::hours(value);
		}		
	}
}



