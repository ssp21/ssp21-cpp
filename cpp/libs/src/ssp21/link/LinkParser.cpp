
#include "ssp21/link/LinkParser.h"

#include "openpal/util/Comparisons.h"
#include "openpal/serialization/Serialization.h"

#include "ssp21/link/CastagnoliCRC32.h"

using namespace openpal;

namespace ssp21
{					
		LinkParser::LinkParser(uint16_t max_payload_size) : 
			max_payload_length_(
				bounded<uint32_t>(
					max_payload_size,
					min_config_link_payload_size,
					max_config_link_payload_size
				)
			),
			buffer_(min_link_frame_size + max_payload_length_),			
			state_(State::wait_sync1),
			num_rx_(0),
			num_crc_error_(0),
			payload_length_(0)
		{
			
		}

		LinkParser::Result LinkParser::parse(RSlice& input)
		{	
			this->num_crc_error_ = 0;

			this->state_ = parse_many(this->state_, input);
			
			return Result(state_ == State::wait_read, num_crc_error_);
		}

		LinkParser::State LinkParser::parse_many(State state, openpal::RSlice& input)
		{
			auto current_state = state;

			while (input.is_not_empty() && (current_state != State::wait_read))
			{
				current_state = parse_one(current_state, input);
			}

			return current_state;
		}

		LinkParser::State LinkParser::parse_one(State state, RSlice& input)
		{
			switch (state)
			{
			case(State::wait_sync1) :
				return parse_sync1(input);
			case(State::wait_sync2) :
				return parse_sync2(input);
			case(State::wait_header) :
				return parse_header(input);
			case(State::wait_body) :
				return parse_body(input);
			default:
				return State::wait_read;
			}
		}

		LinkParser::State LinkParser::parse_sync1(openpal::RSlice& input)
		{
			const auto value = input[0];
			input.advance(1);
			if (value == sync1)
			{
				this->buffer_[0] = value;
				return State::wait_sync2;
			}
			else
			{				
				return State::wait_sync1;
			}			
		}
		
		LinkParser::State LinkParser::parse_sync2(openpal::RSlice& input)
		{
			const auto value = input[0];
			input.advance(1);
			if (value == sync2)
			{
				this->buffer_[1] = value;
				num_rx_ = 2;
				return State::wait_header;				
			}
			else
			{					
				return State::wait_sync1;
			}
		}
		
		LinkParser::State LinkParser::parse_header(openpal::RSlice& input)
		{
			const auto remaining = link_header_total_size - num_rx_;
			const auto num_to_copy = min<uint32_t>(remaining, input.length());
			input.take(num_to_copy).copy_to(buffer_.as_wslice().skip(num_rx_));
			
			num_rx_ += num_to_copy;
			input.advance(num_to_copy);

			if (num_rx_ != link_header_total_size)
			{
				return State::wait_header;
			}

			// now read and validate the header
			auto expected_crc = CastagnoliCRC32::calc(buffer_.as_rslice().take(link_header_fields_size));
			auto actual_crc = UInt32::read(buffer_.as_rslice().skip(link_header_fields_size));

			if (expected_crc != actual_crc)
			{				
				++this->num_crc_error_;

				// reprocess all header bytes except for the first
				auto recursive_input = this->buffer_.as_rslice().take(link_header_total_size).skip(1);

				return parse_many(State::wait_sync1, recursive_input);
			}

			this->addresses_.destination = UInt16::read(buffer_.as_rslice().skip(2));
			this->addresses_.source = UInt16::read(buffer_.as_rslice().skip(4));

			const auto payload_length = UInt16::read(buffer_.as_rslice().skip(6));

			if (payload_length > this->max_payload_length_)
			{			
				return State::wait_sync1;
			}

			this->payload_length_ = payload_length;
			return State::wait_body;
		}
		
		LinkParser::State LinkParser::parse_body(openpal::RSlice& input)
		{
			const auto total_frame_size = link_header_total_size + this->payload_length_ + crc_size;
			const auto remaining = total_frame_size - this->num_rx_;

			const auto num_to_copy = min<uint32_t>(remaining, input.length());
			input.take(num_to_copy).copy_to(buffer_.as_wslice().skip(num_rx_));

			num_rx_ += num_to_copy;
			input.advance(num_to_copy);

			if (num_rx_ != total_frame_size)
			{
				return State::wait_body;
			}
			
			const auto payload_bytes = buffer_.as_rslice().skip(link_header_total_size).take(payload_length_);
			const auto expected_crc = CastagnoliCRC32::calc(payload_bytes);
			const auto actual_crc = UInt32::read(buffer_.as_rslice().skip(link_header_total_size + payload_length_));

			if (expected_crc != actual_crc)
			{
				++this->num_crc_error_;
				return State::wait_sync1;
			}

			this->payload_ = payload_bytes;

			return State::wait_read;
		}

		bool LinkParser::read(Addresses& addresses, RSlice& payload)
		{
			if (state_ != State::wait_read)
			{
				return false;
			}

			state_ = State::wait_sync1;
			num_rx_ = 0;

			addresses = addresses_;
			payload = payload_;

			return true;
		}

}


