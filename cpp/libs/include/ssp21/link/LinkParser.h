
#ifndef SSP21_LINKPARSER_H
#define SSP21_LINKPARSER_H

#include "openpal/util/Uncopyable.h"
#include "openpal/container/Buffer.h"

#include "Addresses.h"

namespace ssp21
{		
	class LinkParser : openpal::Uncopyable
	{
		
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

		class State
		{

		public:

			enum class Value
			{
				wait_sync1,
				wait_sync2,
				wait_header,
				wait_body,
				wait_read
			};
				
			State() {}

			static State wait_sync1() { return State(Value::wait_sync1, 0); }
			static State wait_sync2() { return State(Value::wait_sync2, 1); }
			static State wait_header(uint32_t num) { return State(Value::wait_header, num); }
			static State wait_body(uint32_t num) { return State(Value::wait_body, num); }
			static State wait_read(uint32_t num) { return State(Value::wait_read, num); }
			
			Value value = Value::wait_sync1;			
			uint32_t num_buffered = 0;

			bool is_wait_read() const { return value == Value::wait_read; }

		private:

			State(Value state, uint32_t num_buffered) :
				value(state),
				num_buffered(num_buffered)
			{}
		};

		State parse_one(const State& state, openpal::RSlice& input);
		State parse_many(const State& state, openpal::RSlice& input);

		State parse_sync1(const State& state, openpal::RSlice& input);
		State parse_sync2(const State& state, openpal::RSlice& input);
		State parse_header(const State& state, openpal::RSlice& input);
		State parse_body(const State& state, openpal::RSlice& input);

		LinkParser() = delete;		

		const uint16_t max_payload_length_;
		IReporter* reporter_;
		openpal::Buffer buffer_;

		State state_;		
				
		openpal::RSlice payload_;
		Addresses addresses_;
		uint16_t payload_length_;
	};



}

#endif
