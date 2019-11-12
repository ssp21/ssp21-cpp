#ifndef SSP21PROXY_SERIALSETTINGS_H
#define SSP21PROXY_SERIALSETTINGS_H

#include <asio.hpp>

struct SerialSettings
{
	explicit SerialSettings(const std::string& serial_device) : serial_device(serial_device) {}

	SerialSettings(
		const std::string& serial_device,
		unsigned int baud,
		unsigned int data_bits,
		asio::serial_port::flow_control::type flow_control,
		asio::serial_port::stop_bits::type stop_bits

	) :
		serial_device(serial_device),
		baud(baud),
		data_bits(data_bits),
		flow_control(flow_control),
		stop_bits(stop_bits)
	{}

	std::string serial_device;
	unsigned int baud = 9600;
	unsigned int data_bits = 8;
	asio::serial_port::flow_control::type flow_control = asio::serial_port::flow_control::none;
	asio::serial_port::stop_bits::type stop_bits = asio::serial_port::stop_bits::one;
};

#endif
