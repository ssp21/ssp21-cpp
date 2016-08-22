
#ifndef SSP21_LINKPARSER_H
#define SSP21_LINKPARSER_H

#include "openpal/util/Uncopyable.h"
#include "openpal/container/Buffer.h"

#include "Addresses.h"

namespace ssp21
{	
	const uint8_t sync1 = 0x07;
	const uint8_t sync2 = 0xAA;
	const uint16_t min_config_link_payload_size = 512;
	const uint16_t max_config_link_payload_size = 4092;
	
	
	const uint16_t crc_size = 4;
	const uint16_t link_header_fields_size = 8;
	const uint16_t link_header_total_size = link_header_fields_size + crc_size;
	const uint16_t min_link_frame_size = link_header_total_size + crc_size;

	class LinkParser : openpal::Uncopyable
	{
		enum class State
		{
			wait_sync1,
			wait_sync2,
			wait_header,
			wait_body,
			wait_read
		};

	public:

		struct Stats
		{
			Stats() : num_crc_failures(0)
			{}
			
			uint16_t num_crc_failures;			
		};

		LinkParser(uint16_t max_payload_size);

		bool parse(openpal::RSlice& input, Stats& stats);

		bool read(Addresses& addresses, openpal::RSlice& payload);

	private:

		State parse_one(openpal::RSlice& input, Stats& stats);

		State parse_sync1(openpal::RSlice& input);
		State parse_sync2(openpal::RSlice& input);
		State parse_header(openpal::RSlice& input, Stats& stats);
		State parse_body(openpal::RSlice& input, Stats& stats);

		LinkParser() = delete;		

		State state_;
		uint32_t num_rx_;
		
		const uint16_t max_payload_length_;
		openpal::Buffer buffer_;
		openpal::RSlice payload_;
		Addresses addresses_;
		uint16_t payload_length_;
	};



}

#endif
