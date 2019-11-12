#include "QIXQKDSource.h"

#include <ssp21/util/Exception.h>

#include <qix/AsyncQIXFrameReader.h>

#include "YAMLHelpers.h"

asio::serial_port::flow_control::type get_flow_control(const YAML::Node& node)
{
    const auto value = yaml::optional_string(node, "flow_control", "none");
    
    if (value == "none") {
        return asio::serial_port::flow_control::type::none;
    }

    if (value == "hardware") {
        return asio::serial_port::flow_control::type::hardware;
    }

    if (value == "software") {
        return asio::serial_port::flow_control::type::software;
    }

    throw yaml::YAMLException(node.Mark(), "Unknown flow_control type: ", value);
}

asio::serial_port::stop_bits::type get_stop_bits(const YAML::Node& node)
{
    const auto value = yaml::optional_string(node, "stop_bits", "one");

    if (value == "one") {
        return asio::serial_port::stop_bits::type::one;
    }

    if (value == "onepointfive") {
        return asio::serial_port::stop_bits::type::onepointfive;
    }

    if (value == "two") {
        return asio::serial_port::stop_bits::type::two;
    }

    throw yaml::YAMLException(node.Mark(), "Unknown stop_bits type: ", value);
}

SerialSettings read_serial_settings(const YAML::Node& node)
{
    return SerialSettings(
        yaml::require_string(node, "device"),
        yaml::optional_integer<unsigned int>(node, "baud_rate", 9600),
        yaml::optional_integer<unsigned int>(node, "data_bits", 8),
        get_flow_control(node),
        get_stop_bits(node)
    );
}

QIXQKDSource::FrameHandler::FrameHandler(uint16_t key_cycle_length) : key_cycle_length(key_cycle_length)
{}

void QIXQKDSource::FrameHandler::handle(const QIXFrame& frame)
{
    if (frame.status == QIXFrame::Status::ok)
    {
        const uint16_t modulo = frame.key_id % this->key_cycle_length;
        const auto subscriber = this->subscribers.find(modulo);
        if (subscriber != this->subscribers.end())
        {
            subscriber->second->add_key(frame);
        }
    }        
}

QIXQKDSource::QIXQKDSource(const YAML::Node& node, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, log4cpp::Logger& logger) :
	handler(std::make_shared<FrameHandler>(yaml::require_integer<uint16_t>(node, "key_cycle_length")))	
{
   const auto settings = read_serial_settings(yaml::require(node, "serial"));

   qix::async::start_frame_reader(this->handler, logger, executor, settings);

}

std::shared_ptr<QIXKeyStore> QIXQKDSource::bind_key_store(uint16_t modulo, uint16_t max_cached_keys)
{
    const auto existing = this->handler->subscribers.find(modulo);
    if (existing != this->handler->subscribers.end()) {
        throw ssp21::Exception("Duplicate modulos for QKD subscriber: ", modulo);
    }

    const auto ret = std::make_shared<QIXKeyStore>(max_cached_keys);
    this->handler->subscribers[modulo] = ret;
    return ret;
}

std::shared_ptr<ssp21::IKeyLookup> QIXQKDSource::bind_responder_key_lookup(uint16_t modulos, uint16_t max_cached_keys)
{
    return this->bind_key_store(modulos, max_cached_keys);
}
	
std::shared_ptr<ssp21::IKeySource> QIXQKDSource::bind_initiator_key_source(uint16_t modulos, uint16_t max_cached_keys)
{
    return this->bind_key_store(modulos, max_cached_keys);
}