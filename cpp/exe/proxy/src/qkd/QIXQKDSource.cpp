#include "QIXQKDSource.h"

#include <ssp21/util/Exception.h>
#include <ssp21/stack/LogLevels.h>

#include <qix/AsyncQIXFrameReader.h>

#include <log4cpp/LogMacros.h>
#include <cmath>

#include "YAMLHelpers.h"

using namespace std::chrono;


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

QIXQKDSource::FrameHandler::FrameHandler(const YAML::Node& node, log4cpp::Logger& logger) :
    num_subscribers(yaml::require_integer<uint16_t>(node, "num_subscribers")),
    key_metric_update_rate(yaml::require_duration(node, "key_metric_update_rate")),
    key_metric_bin_size(yaml::require_integer<uint16_t>(node, "key_metric_bin_size")),
    logger(logger),
    last_metric_update_time(high_resolution_clock::time_point::min())
{

}

void QIXQKDSource::FrameHandler::handle(const QIXFrame& frame)
{
    const auto now = high_resolution_clock::now();
    const auto elapsed = now - this->last_metric_update_time;

    if (frame.status != QIXFrame::Status::ok)
    {
        SIMPLE_LOG_BLOCK(this->logger, ssp21::levels::warn, "received QIX frame with bad status");
        return;
    }

    // route to the correct subscriber first before re-calculating metrics
    const uint16_t subscriber_id = frame.key_id % this->num_subscribers;
    const auto subscriber = this->subscribers.find(subscriber_id);
    if (subscriber != this->subscribers.end())
    {
        subscriber->second->add_key(frame);
    }

    // add the time to the bin and trim the bin if necessary
    this->key_data_bin.push_back(now);
    if (this->key_data_bin.size() > this->key_metric_bin_size)
    {
        this->key_data_bin.pop_front();
    }

    // do a metric update if the bin is at capacity
    // and enough time has elapsed since the last update
    if (this->key_data_bin.size() == this->key_metric_bin_size
        && elapsed > this->key_metric_update_rate)
    {
        this->last_metric_update_time = now;

        const auto mean = this->calc_mean_time_between_keys();
        const auto std_dev = this->calc_std_dev_of_time_between_keys(mean);

        FORMAT_LOG_BLOCK(this->logger, ssp21::levels::metric, "mean_time_between_keys: %f", mean);
        FORMAT_LOG_BLOCK(this->logger, ssp21::levels::metric, "std_dev_of_time_between_keys: %f", std_dev);
    }
}

template<class Iter, class F>
void for_each_pair(Iter start, Iter end, F func)
{
    if (start == end)
    {
        return;
    }
        
    Iter next = start;
    ++next;

    for (; next != end; ++start, ++next)
    {
        func(*start, *next);
    }
}

double fractional_seconds(std::chrono::high_resolution_clock::duration duration)
{
    const auto ratio = std::chrono::high_resolution_clock::duration::period();
    
    return static_cast<double>(duration.count()) * static_cast<double>(ratio.den) / static_cast<double>(ratio.num);
}

double QIXQKDSource::FrameHandler::calc_mean_time_between_keys()
{
    // don't divide by zero
    if (this->key_data_bin.empty())
    {
        return std::numeric_limits<double>::infinity();
    }

    double sum = 0.0;

    for_each_pair(
        this->key_data_bin.begin(),
        this->key_data_bin.end(), 
        [&](high_resolution_clock::time_point t1, high_resolution_clock::time_point t2) {
            sum += fractional_seconds(t2 - t1);
        }
    );

    return sum / static_cast<double>(this->key_data_bin.size());
}

double QIXQKDSource::FrameHandler::calc_std_dev_of_time_between_keys(double mean)
{
    // don't divide by zero
    if (this->key_data_bin.empty())
    {
        return std::numeric_limits<double>::infinity();
    }

    double sum_of_squares = 0.0;

    for_each_pair(
        this->key_data_bin.begin(),
        this->key_data_bin.end(),
        [&](auto t1, auto t2) {
            const auto diff = fractional_seconds(t2 - t1);
            const auto diff_minus_mean = diff - mean;
            sum_of_squares += diff_minus_mean * diff_minus_mean;
        }
    );

    return std::sqrt(sum_of_squares / static_cast<double>(this->key_data_bin.size()));
}

QIXQKDSource::QIXQKDSource(const YAML::Node& node, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, log4cpp::Logger& logger) :
	handler(std::make_shared<FrameHandler>(node, logger))
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