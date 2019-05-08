
#include "qix/QIXFrameReader.h"


#include <ssp21/stack/LogLevels.h>
#include <ssp21/crypto/Crypto.h>
#include <ssp21/link/CastagnoliCRC32.h>

#include <sodium/CryptoBackend.h>

#include <log4cpp/LogMacros.h>
#include <log4cpp/ConsolePrettyPrinter.h>

#include <argagg/argagg.hpp>
#include <ser4cpp/serialization/BigEndian.h>
#include <ser4cpp/container/StaticBuffer.h>
#include <ser4cpp/serialization/SerializationTemplates.h>

#include <iostream>
#include <asio.hpp>

using namespace ser4cpp;
using namespace ssp21;

class QIXPrinter : public IQIXFrameHandler
{
    virtual void handle(const QIXFrame& frame) override
    {
        std::cout << "Received frame w/ id: " << frame.key_id << std::endl;
    }
};

int read_frames(const std::string& serial_port);

int write_frames(const std::string& serial_port, uint16_t frames_per_sec);

std::string get_port(const argagg::parser_results& results);

uint16_t get_key_rate(const argagg::parser_results& results);

int main(int argc, char*  argv[])
{
	argagg::parser parser {{
		{ "help", { "-h", "--help" }, "shows this help message", 0 },
		{ "read", { "-r", "--read" }, "read QIX frames", 0 },
		{ "write", { "-w", "--write" }, "read QIX frames", 0 },
		{ "port", { "-p", "--port" }, "serial port", 1 },
		{ "rate", { "-t", "--rate" }, "number of keys per second (write only) - defaults to 1", 1 },
	}};

	try {
		
		const auto results = parser.parse(argc, argv);

		const auto port = get_port(results);

		if (results.has_option("read")) {
			return read_frames(port);
		}
		else if (results.has_option("write")) {			
			return write_frames(port, get_key_rate(results));
		}
		else {
			throw std::runtime_error("You must specify read or write mode");
		}		
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
		
		return -1;
	}		
}

int read_frames(const std::string& serial_port)
{
	try
	{
		QIXFrameReader reader(
			std::make_shared<QIXPrinter>(),
			log4cpp::Logger(std::make_shared<log4cpp::ConsolePrettyPrinter>(), Module::id, "qix-reader", log4cpp::LogLevels(~0)),
			"/dev/ttyS0"
		);

		std::cout << "waiting for QIX frames" << std::endl;
		std::cout << "press <enter> to terminate" << std::endl;
		std::cin.ignore();
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}

	return 0;
}

int write_frames(const std::string& serial_port, uint16_t frames_per_sec)
{
	if (!sodium::CryptoBackend::initialize()) {
		throw std::runtime_error("can't initialize sodium backend");
	}

	asio::io_service service;
	asio::serial_port port(service);

	port.open(serial_port);

	uint64_t frame_count = 0;

	StaticBuffer<uint32_t, 47> frame;
	StaticBuffer<uint32_t, 32> random_key;
	
	
	while (true) {

		// fill up a new random key
		ssp21::Crypto::gen_random(random_key.as_wseq());

		auto dest = frame.as_wseq();

		UInt8::write_to(dest, 0x5A);
		UInt8::write_to(dest, 0xA5);
		UInt64::write_to(dest, frame_count);		
		dest.copy_from(random_key.as_seq());
		UInt8::write_to(dest, 0x01); // status - OK

		// write the CRC
		UInt32::write_to(
			dest,
			// skip the sync bytes, calculate over count(8) + key(32) + status(1) == 41 bytes
			CastagnoliCRC32::calc(frame.as_seq().skip(2).take(41))
		);

		asio::write(port, asio::buffer(dest, dest.length()));

		std::cout << "wrote frame # " << frame_count << std::endl;

		++frame_count;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

std::string get_port(const argagg::parser_results& results)
{
	if (!results.has_option("port")) {
		throw std::runtime_error("you must specify the serial port");
	}

	if (results["port"].count() != 1) {
		throw std::runtime_error("port argument expects a single value");
	}

	return results["port"][0];
}

uint16_t get_key_rate(const argagg::parser_results& results)
{
	if (results.has_option("key_rate")) {
		return results["key_rate"][0];
	}
	else {
		return 1;
	}
}