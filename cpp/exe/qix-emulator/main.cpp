

#include <ssp21/stack/LogLevels.h>
#include <ssp21/crypto/Crypto.h>
#include <ssp21/link/CastagnoliCRC32.h>

#include <sodium/CryptoBackend.h>

#include <log4cpp/LogMacros.h>
#include <log4cpp/ConsolePrettyPrinter.h>

#include <argagg/argagg.hpp>

#include <ser4cpp/container/StaticBuffer.h>
#include <ser4cpp/util/HexConversions.h>
#include <ser4cpp/serialization/BigEndian.h>

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

int read_frames(const std::vector<std::string>& ports);

int write_random_frames(const std::vector<std::string>& ports, uint64_t frame_count, uint16_t frames_per_sec);

int write_time_based_frames(const std::vector<std::string>& ports, uint64_t frame_count, uint32_t ms_modulo);

std::vector<std::string> get_ports(const argagg::parser_results& results);

uint16_t get_key_rate(const argagg::parser_results& results);

uint64_t get_frame_count(const argagg::parser_results& results);

uint64_t to_ms_since_epoch(std::chrono::system_clock::time_point time);

int main(int argc, char*  argv[])
{
	argagg::parser parser {{
		{ "help", { "-h", "--help" }, "shows this help message", 0 },
		{ "read", { "-r", "--read" }, "read QIX frames", 0 },
		{ "write", { "-w", "--write" }, "write random QIX frames", 0 },
        { "twrite", { "-m", "--twrite" }, "write time-based QIX frames modulo n milliseconds", 1 },
		{ "port", { "-p", "--port" }, "serial port", 1 },
		{ "rate", { "-t", "--rate" }, "number of keys per second (write only) - defaults to 1", 1 },
        { "count", { "-c", "--count" }, "number of frames to transmit (write only) - defaults to 2^64 -1", 1 },
	}};

	try {
		
		const auto results = parser.parse(argc, argv);

		if (results.has_option("help"))
		{
			std::cout << parser << std::endl;
			return 0;
		}

		const auto ports = get_ports(results);

		if (results.has_option("read")) {
			return read_frames(ports);
		}
		else if (results.has_option("write")) {			
			return write_random_frames(ports, get_frame_count(results), get_key_rate(results));
		}
        else if (results.has_option("twrite")) {
            return write_time_based_frames(ports, get_frame_count(results), results["twrite"][0].as<uint32_t>());
        }
		else {
			throw std::runtime_error("You must specify read or write mode");
		}		
	}
	catch (const std::exception & ex)
	{
		std::cerr << ex.what() << std::endl << std::endl;
		std::cout << parser << std::endl;
		return -1;
	}		
}

int read_frames(const std::vector<std::string>& ports)
{
	if (ports.size() != 1) {
		throw std::runtime_error("Can only read from a single serial port");
	}

	try
	{
		QIXFrameReader reader(
			std::make_shared<QIXPrinter>(),
			log4cpp::Logger(std::make_shared<log4cpp::ConsolePrettyPrinter>(), Module::id, "qix-reader", log4cpp::LogLevels(0xFF)),
            ports[0]
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

int write_random_frames(const std::vector<std::string>& ports, uint64_t frame_count, uint16_t frames_per_sec)
{
	if (ports.empty()) {
		throw std::runtime_error("you must specify at least one serial port");
	}

    if(frames_per_sec == 0) {
        throw std::runtime_error("frame rate cannot be zero");
    }

    const auto frame_delay = std::chrono::milliseconds(1000 / frames_per_sec);


	if (!sodium::CryptoBackend::initialize()) {
		throw std::runtime_error("can't initialize sodium backend");
	}

	std::vector<std::unique_ptr<QIXFrameWriter>> writers;

	for (auto port : ports) {
		writers.push_back(std::make_unique<QIXFrameWriter>(port));
	}

	StaticBuffer<uint32_t, 32> random_key;

	for (uint64_t i = 0; i < frame_count; ++i) {

	    const auto start = std::chrono::steady_clock::now();

		// fill up a new random key
		ssp21::Crypto::gen_random(random_key.as_wseq());

		const QIXFrame frame { i , random_key.as_seq(), QIXFrame::Status::ok };

		for (const auto& writer : writers) {
			writer->write(frame);
		}

        std::cout << "wrote: " << frame.key_id << " - " << get_status_string(frame.status) << " - " << HexConversions::to_hex(frame.key_data) << std::endl;

        const auto elapsed = start - std::chrono::steady_clock::now();

        if(elapsed < frame_delay) {
            std::this_thread::sleep_for(frame_delay - elapsed);
        }

	}

	return 0;
}

int write_time_based_frames(const std::vector<std::string>& ports, uint64_t frame_count, uint32_t ms_modulo)
{
    if (ports.empty()) {
        throw std::runtime_error("you must specify at least one serial port");
    }

    if (ms_modulo < 100) {
        throw std::runtime_error("twrite moduluo cannot be less than 100");
    }

    if (!sodium::CryptoBackend::initialize()) {
        throw std::runtime_error("can't initialize sodium backend");
    }

    std::vector<std::unique_ptr<QIXFrameWriter>> writers;

    for (auto port : ports) {
        writers.push_back(std::make_unique<QIXFrameWriter>(port));
    }

    HashOutput key_buffer;
    StaticBuffer<uint32_t, 8> key_id_buffer;

    for (uint64_t i = 0; i < frame_count; ++i) {

        const auto now = std::chrono::system_clock::now();
        const auto now_ms = to_ms_since_epoch(now);

        // sleep until the next modulo event
        const auto next = now + std::chrono::milliseconds(ms_modulo - (now_ms % ms_modulo));
        std::this_thread::sleep_until(next);

        // the next millisecond modulo is the key id
        const auto key_id = to_ms_since_epoch(next);

        {
            auto output = key_id_buffer.as_wseq();
            // the key itself is the SHA2 hash of the key id
            ser4cpp::UInt64::write_to(output, key_id);  //serialize as BigEndian
            ssp21::Crypto::hash_sha256({ key_id_buffer.as_seq() }, key_buffer);
        }

        const QIXFrame frame{ key_id , key_buffer.as_seq(), QIXFrame::Status::ok };

        for (const auto& writer : writers) {
            writer->write(frame);
        }

        std::cout << "wrote: " << frame.key_id << " - " << get_status_string(frame.status) << " - " << HexConversions::to_hex(frame.key_data) << std::endl;
    }

    return 0;
}

std::vector<std::string> get_ports(const argagg::parser_results& results)
{
	if (!results.has_option("port")) {
		throw std::runtime_error("you must specify the serial port");
	}

	std::vector<std::string> ports;

	for (const auto& value : results["port"].all)
	{
		ports.push_back(value);
	}

	return ports;
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

uint64_t to_ms_since_epoch(std::chrono::system_clock::time_point time)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
}
