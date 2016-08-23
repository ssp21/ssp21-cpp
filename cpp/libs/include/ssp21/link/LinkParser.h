
#ifndef SSP21_LINKPARSER_H
#define SSP21_LINKPARSER_H

#include "openpal/util/Uncopyable.h"
#include "openpal/container/Buffer.h"

#include "Addresses.h"

namespace ssp21
{	
	const uint8_t sync1 = 0x07;
	const uint8_t sync2 = 0xAA;	
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

		class IReporter
		{
		public:
			virtual void on_bad_header_crc(uint32_t expected, uint32_t actual) = 0;
			virtual void on_bad_body_crc(uint32_t expected, uint32_t actual) = 0;
			virtual void on_bad_body_length(uint32_t max_allowed, uint32_t actual) = 0;
		};		

		LinkParser(uint16_t max_payload_length, IReporter& reporter);

		bool parse(openpal::RSlice& input);

		bool read(Addresses& addresses, openpal::RSlice& payload);

	private:

		struct FullState
		{
			FullState(State state, uint32_t num_buffered) :
				value(state),
				num_buffered(num_buffered)
			{}	

			FullState() {}
			
			State value = State::wait_sync1;			
			uint32_t num_buffered = 0;
		};

		FullState parse_one(const FullState& state, openpal::RSlice& input);
		FullState parse_many(const FullState& state, openpal::RSlice& input);

		FullState parse_sync1(const FullState& state, openpal::RSlice& input);
		FullState parse_sync2(const FullState& state, openpal::RSlice& input);
		FullState parse_header(const FullState& state, openpal::RSlice& input);
		FullState parse_body(const FullState& state, openpal::RSlice& input);

		LinkParser() = delete;		

		const uint16_t max_payload_length_;
		IReporter* reporter_;
		openpal::Buffer buffer_;

		FullState state_;		
				
		openpal::RSlice payload_;
		Addresses addresses_;
		uint16_t payload_length_;
	};



}

#endif
