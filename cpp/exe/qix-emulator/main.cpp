

#include <ssp21/stack/LogLevels.h>
#include <ssp21/crypto/Crypto.h>
#include <ssp21/link/CastagnoliCRC32.h>

#include <sodium/CryptoBackend.h>

#include <log4cpp/LogMacros.h>
#include <log4cpp/ConsolePrettyPrinter.h>

#include <argagg/argagg.hpp>

#include <ser4cpp/container/StaticBuffer.h>
#include <ser4cpp/util/HexConversions.h>

#include <qix/QIXFrameReader.h>
#include <qix/QIXFrameWriter.h>

#include <iostream>

using namespace ser4cpp;
using namespace ssp21;

const char* get_status_string(QIXFrame::Status status)
{
    switch(status)
    {
        case(QIXFrame::Status::ok):
            return "ok";
        case(QIXFrame::Status::key_compromised):
            return "compromised";
        default:
            return "undefined";
    }
}

class QIXPrinter : public IQIXFrameHandler
{
    void handle(const QIXFrame& frame) override
    {
        std::cout << "read: " << frame.key_id << " - " << get_status_string(frame.status) << " - " << HexConversions::to_hex(frame.key_data) << std::endl;
    }
};

int read_frames(const std::string& serial_port);

int write_frames(const std::string& serial_port, uint64_t frame_count, uint16_t frames_per_sec);

std::string get_port(const argagg::parser_results& results);

uint16_t get_key_rate(const argagg::parser_results& results);

uint64_t get_frame_count(const argagg::parser_results& results);

int main(int argc, char*  argv[])
{
	argagg::parser parser {{
		{ "help", { "-h", "--help" }, "shows this help message", 0 },
		{ "read", { "-r", "--read" }, "read QIX frames", 0 },
		{ "write", { "-w", "--write" }, "read QIX frames", 0 },
		{ "port", { "-p", "--port" }, "serial port", 1 },
		{ "rate", { "-t", "--rate" }, "number of keys per second (write only) - defaults to 1", 1 },
        { "count", { "-c", "--count" }, "number of frames to transmit (write only) - defaults to 2^64 -1", 1 },
	}};

	try {
		
		const auto results = parser.parse(argc, argv);

		const auto port = get_port(results);

		if (results.has_option("read")) {
			return read_frames(port);
		}
		else if (results.has_option("write")) {			
			return write_frames(port, get_frame_count(results), get_key_rate(results));
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
			log4cpp::Logger(std::make_shared<log4cpp::ConsolePrettyPrinter>(), Module::id, "qix-reader", log4cpp::LogLevels(0xFF)),
            serial_port
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

int write_frames(const std::string& serial_port, uint64_t frame_count, uint16_t frames_per_sec)
{
	if (!sodium::CryptoBackend::initialize()) {
		throw std::runtime_error("can't initialize sodium backend");
	}

	QIXFrameWriter writer(serial_port);

	StaticBuffer<uint32_t, 32> random_key;

	for (uint64_t i = 0; i < frame_count; ++i) {

		// fill up a new random key
		ssp21::Crypto::gen_random(random_key.as_wseq());

		const QIXFrame frame { i , random_key.as_seq(), QIXFrame::Status::ok };

		writer.write(frame);

        std::cout << "wrote: " << frame.key_id << " - " << get_status_string(frame.status) << " - " << HexConversions::to_hex(frame.key_data) << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
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
	if (results.has_option("rate")) {
		return results["rate"][0];
	}
	else {
		return 1;
	}
}

uint64_t get_frame_count(const argagg::parser_results& results)
{
    if (results.has_option("count")) {
        return results["count"][0];
    }
    else {
        return std::numeric_limits<uint64_t>::max();
    }
}