
#include "qix/QIXFrameReader.h"

#include "log4cpp/ConsolePrettyPrinter.h"
#include "ssp21/stack/LogLevels.h"

#include "log4cpp/LogMacros.h"

#include <argagg/argagg.hpp>

#include <iostream>

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
			throw std::exception("You must specify read or write mode");
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
	throw std::exception("writing frames not yet supported");
}

std::string get_port(const argagg::parser_results& results)
{
	if (!results.has_option("port")) {
		throw std::exception("you must specify the serial port");
	}

	if (results["port"].count() != 1) {
		throw std::exception("port argument expects a single value");
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