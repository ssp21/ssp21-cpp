
#include "ssp21/link/LinkParser.h"

#include "openpal/util/Comparisons.h"
#include "openpal/serialization/Serialization.h"

#include "ssp21/link/CastagnoliCRC32.h"
#include "ssp21/link/LinkConstants.h"

using namespace openpal;

namespace ssp21
{					
		LinkParser::LinkParser(uint16_t max_payload_length, IReporter& reporter) :
			max_payload_length_(
				min<uint32_t>(
					max_payload_length,					
					consts::max_config_link_payload_size
				)
			),
			reporter_(&reporter),
			buffer_(consts::min_link_frame_size + max_payload_length_),
			payload_length_(0)
		{
			
		}

		bool LinkParser::parse(RSlice& input)
		{							
			this->state_ = parse_many(this->state_, input);
			
			return state_.value == State::wait_read;
		}

		LinkParser::FullState LinkParser::parse_many(const FullState& state, openpal::RSlice& input)
		{
			auto current_state = state;

			while (input.is_not_empty() && (current_state.value != State::wait_read))
			{
                current_state = parse_one(current_state, input);
            }

			return current_state;
		}

		LinkParser::FullState LinkParser::parse_one(const FullState& state, RSlice& input)
		{
			switch (state.value)
			{
			case(State::wait_sync1) :
				return parse_sync1(state, input);
			case(State::wait_sync2) :
				return parse_sync2(state, input);
			case(State::wait_header) :
				return parse_header(state, input);
			case(State::wait_body) :
				return parse_body(state, input);
			default:
				return state; // wait_read
			}
		}

		LinkParser::FullState LinkParser::parse_sync1(const FullState& state, openpal::RSlice& input)
		{
			const auto value = input[0];
			input.advance(1);
			if (value == consts::sync1)
			{
				this->buffer_[0] = value;
				return FullState(State::wait_sync2, 1);
			}
			else
			{				
				return FullState(State::wait_sync1, 0);
			}			
		}
		
		LinkParser::FullState LinkParser::parse_sync2(const FullState& state, openpal::RSlice& input)
		{
			const auto value = input[0];
			input.advance(1);
			if (value == consts::sync2)
			{
				this->buffer_[1] = value;				
				return FullState(State::wait_header, 2);				
			}
			else
			{					
				return FullState(State::wait_header, 0);
			}
		}
		
		LinkParser::FullState LinkParser::parse_header(const FullState& state, openpal::RSlice& input)
		{
			const auto remaining = consts::link_header_total_size - state.num_buffered;
			const auto num_to_copy = min<uint32_t>(remaining, input.length());
			auto dest = buffer_.as_wslice().skip(state.num_buffered);
			
			input.take(num_to_copy).move_to(dest);
			input.advance(num_to_copy);

			const auto new_num_buffered = num_to_copy + state.num_buffered;

			if (new_num_buffered != consts::link_header_total_size)
			{
				return FullState(State::wait_header, new_num_buffered);
			}

			// now read and validate the header
			auto expected_crc = CastagnoliCRC32::calc(buffer_.as_rslice().take(consts::link_header_fields_size));
			auto actual_crc = UInt32::read(buffer_.as_rslice().skip(consts::link_header_fields_size));

			if (expected_crc != actual_crc)
			{				
				reporter_->on_bad_header_crc(expected_crc, actual_crc);
				
				auto header = this->buffer_.as_rslice().take(consts::link_header_total_size).skip(2);

				// reprocess all header bytes except for the synchronization bytes.
				// 
				// Since this segment has length (link_header_total_size - 2) we're
				// guaranteed it'll all be processed from wait_sync1
				return parse_many(FullState(State::wait_sync1, 0), header);
			}

			this->addresses_.destination = UInt16::read(buffer_.as_rslice().skip(2));
			this->addresses_.source = UInt16::read(buffer_.as_rslice().skip(4));

			const auto payload_length = UInt16::read(buffer_.as_rslice().skip(6));

			if (payload_length > this->max_payload_length_)
			{			
				this->reporter_->on_bad_body_length(this->max_payload_length_, payload_length);
				return FullState(State::wait_sync1, 0);
			}

			this->payload_length_ = payload_length;

			return FullState(State::wait_body, new_num_buffered);
		}
		
		LinkParser::FullState LinkParser::parse_body(const FullState& state, openpal::RSlice& input)
		{
			const uint32_t total_frame_size = consts::link_header_total_size + this->payload_length_ + consts::crc_size;
			const uint32_t remaining = total_frame_size - state.num_buffered;

			const auto num_to_copy = min<uint32_t>(remaining, input.length());
			auto dest = buffer_.as_wslice().skip(state.num_buffered);
			input.take(num_to_copy).copy_to(dest);			
			input.advance(num_to_copy);
			const auto new_num_buffered = num_to_copy + state.num_buffered;

			if (new_num_buffered != total_frame_size)
			{
				return FullState(State::wait_body, new_num_buffered);
			}
			
			const auto payload_bytes = buffer_.as_rslice().skip(consts::link_header_total_size).take(payload_length_);
			const auto expected_crc = CastagnoliCRC32::calc(payload_bytes);
			const auto actual_crc = UInt32::read(buffer_.as_rslice().skip(consts::link_header_total_size + payload_length_));

			if (expected_crc != actual_crc)
			{
				reporter_->on_bad_body_crc(expected_crc, actual_crc);
				return FullState(State::wait_sync1, 0);
			}

			this->payload_ = payload_bytes;

			return FullState(State::wait_read, new_num_buffered);
		}

		bool LinkParser::read(Addresses& addresses, RSlice& payload)
		{
			if (state_.value != State::wait_read)
			{
				return false;
			}

			state_ = FullState(State::wait_sync1, 0);
			
			addresses = addresses_;
			payload = payload_;

			return true;
		}

}


