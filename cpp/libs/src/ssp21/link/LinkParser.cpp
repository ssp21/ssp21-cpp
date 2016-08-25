
#include "ssp21/link/LinkParser.h"

#include "openpal/util/Comparisons.h"
#include "openpal/serialization/Serialization.h"

#include "ssp21/link/CastagnoliCRC32.h"
#include "ssp21/link/LinkConstants.h"

using namespace openpal;

namespace ssp21
{
    LinkParser::Context::Context(uint16_t max_payload_length, IReporter& reporter) :
        max_payload_length(
            min<uint32_t>(
                max_payload_length,
                consts::max_config_link_payload_size
            )
        ),
        reporter(&reporter),
        buffer(consts::min_link_frame_size + max_payload_length)
    {}

    LinkParser::LinkParser(uint16_t max_payload_length, IReporter& reporter) :
        context_(max_payload_length, reporter)
    {

    }

    bool LinkParser::parse(RSlice& input)
    {
        this->state_ = parse_many(this->state_, this->context_, input);

        return state_.value == State::Value::wait_read;
    }

    LinkParser::State LinkParser::parse_many(const State& state, Context& ctx, openpal::RSlice& input)
    {
        auto current_state = state;

        while (input.is_not_empty() && !current_state.is_wait_read())
        {
            current_state = parse_one(current_state, ctx, input);
        }

        return current_state;
    }

    LinkParser::State LinkParser::parse_one(const State& state, Context& ctx, RSlice& input)
    {
        switch (state.value)
        {
        case(State::Value::wait_sync1) :
            return parse_sync1(state, ctx, input);
        case(State::Value::wait_sync2) :
            return parse_sync2(state, ctx, input);
        case(State::Value::wait_header) :
            return parse_header(state, ctx, input);
        case(State::Value::wait_body) :
            return parse_body(state, ctx, input);
        default:
            return state; // wait_read
        }
    }

    LinkParser::State LinkParser::parse_sync1(const State& state, Context& ctx, openpal::RSlice& input)
    {
        // scan the input one byte at a time looking
        // for the first synchronization character
        while (input.is_not_empty())
        {
            const auto value = input[0];
            input.advance(1);
            if (value == consts::sync1)
            {
                ctx.buffer[0] = value;
                return State::wait_sync2();
            }
        }

        return State::wait_sync1();
    }

    LinkParser::State LinkParser::parse_sync2(const State& state, Context& ctx, openpal::RSlice& input)
    {
        const auto value = input[0];
        input.advance(1);
        if (value == consts::sync2)
        {
            ctx.buffer[1] = value;
            return State::wait_header(2);
        }
        else
        {
            return State::wait_sync1();
        }
    }

    LinkParser::State LinkParser::parse_header(const State& state, Context& ctx, openpal::RSlice& input)
    {
        const auto remaining = consts::link_header_total_size - state.num_buffered;
        const auto num_to_copy = min<uint32_t>(remaining, input.length());
        auto dest = ctx.buffer.as_wslice().skip(state.num_buffered);

        input.take(num_to_copy).move_to(dest);
        input.advance(num_to_copy);

        const auto new_num_buffered = num_to_copy + state.num_buffered;

        if (new_num_buffered != consts::link_header_total_size)
        {
            return State::wait_header(new_num_buffered);
        }

        // now read and validate the header
        auto expected_crc = CastagnoliCRC32::calc(ctx.buffer.as_rslice().take(consts::link_header_fields_size));
        auto actual_crc = UInt32::read(ctx.buffer.as_rslice().skip(consts::link_header_fields_size));

        if (expected_crc != actual_crc)
        {
            ctx.reporter->on_bad_header_crc(expected_crc, actual_crc);

            auto header = ctx.buffer.as_rslice().take(consts::link_header_total_size).skip(2);

            // reprocess all header bytes except for the synchronization bytes.
            //
            // Since this segment has length (link_header_total_size - 2) we're
            // guaranteed it'll all be processed from wait_sync1
            return parse_many(State::wait_sync1(), ctx, header);
        }

        ctx.message.addresses.destination = UInt16::read(ctx.buffer.as_rslice().skip(2));
        ctx.message.addresses.source = UInt16::read(ctx.buffer.as_rslice().skip(4));

        const auto payload_length = UInt16::read(ctx.buffer.as_rslice().skip(6));

        if (payload_length > ctx.max_payload_length)
        {
            ctx.reporter->on_bad_body_length(ctx.max_payload_length, payload_length);
            return State::wait_sync1();
        }

        ctx.payload_length = payload_length;

        return State::wait_body(new_num_buffered);
    }

    LinkParser::State LinkParser::parse_body(const State& state, Context& ctx, openpal::RSlice& input)
    {
        const uint32_t total_frame_size = consts::link_header_total_size + ctx.payload_length + consts::crc_size;
        const uint32_t remaining = total_frame_size - state.num_buffered;

        const auto num_to_copy = min<uint32_t>(remaining, input.length());
        auto dest = ctx.buffer.as_wslice().skip(state.num_buffered);
        input.take(num_to_copy).copy_to(dest);
        input.advance(num_to_copy);
        const auto new_num_buffered = num_to_copy + state.num_buffered;

        if (new_num_buffered != total_frame_size)
        {
            return State::wait_body(new_num_buffered);
        }

        const auto payload_bytes = ctx.buffer.as_rslice().skip(consts::link_header_total_size).take(ctx.payload_length);
        const auto expected_crc = CastagnoliCRC32::calc(payload_bytes);
        const auto actual_crc = UInt32::read(ctx.buffer.as_rslice().skip(consts::link_header_total_size + ctx.payload_length));

        if (expected_crc != actual_crc)
        {
            ctx.reporter->on_bad_body_crc(expected_crc, actual_crc);
            return State::wait_sync1();
        }

        ctx.message.payload = payload_bytes;

        return State::wait_read(new_num_buffered);
    }

}


