
#include "link/LinkParser.h"

#include "openpal/util/Comparisons.h"
#include "openpal/serialization/BigEndian.h"

#include "link/CastagnoliCRC32.h"
#include "ssp21/link/LinkConstants.h"

using namespace openpal;

namespace ssp21
{
    LinkParser::Context::Context(uint16_t max_payload_length, IReporter& reporter) :
        max_payload_length(
            min<uint32_t>(
                max_payload_length,
                consts::link::max_config_payload_size
            )
        ),
        reporter(&reporter),
        buffer(consts::link::min_frame_size + max_payload_length)
    {}

    LinkParser::LinkParser(uint16_t max_payload_length, IReporter& reporter) :
        context(max_payload_length, reporter)
    {

    }

    void LinkParser::reset()
    {
        this->state = State::wait_sync1();
    }

    bool LinkParser::parse(seq32_t& input)
    {
        this->state = parse_many(this->state, this->context, input);

        return state.value == State::Value::wait_read;
    }

    LinkParser::State LinkParser::parse_many(const State& state, Context& ctx, seq32_t& input)
    {
        auto current_state = state;

        while (input.is_not_empty() && !current_state.is_wait_read())
        {
            current_state = parse_one(current_state, ctx, input);
        }

        return current_state;
    }

    LinkParser::State LinkParser::parse_one(const State& state, Context& ctx, seq32_t& input)
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

    LinkParser::State LinkParser::parse_sync1(const State& state, Context& ctx, seq32_t& input)
    {
        // scan the input one byte at a time looking
        // for the first synchronization character
        while (input.is_not_empty())
        {
            const auto value = input[0];
            input.advance(1);
            if (value == consts::link::sync1)
            {
                ctx.buffer[0] = value;
                return State::wait_sync2();
            }
        }

        return State::wait_sync1();
    }

    LinkParser::State LinkParser::parse_sync2(const State& state, Context& ctx, seq32_t& input)
    {
        const auto value = input[0];
        input.advance(1);
        if (value == consts::link::sync2)
        {
            ctx.buffer[1] = value;
            return State::wait_header(2);
        }
        else
        {
            return State::wait_sync1();
        }
    }

    LinkParser::State LinkParser::parse_header(const State& state, Context& ctx, seq32_t& input)
    {
        const auto new_num_buffered = transfer_data(state, ctx, input, consts::link::header_total_size);

        if (new_num_buffered != consts::link::header_total_size)
        {
            return State::wait_header(new_num_buffered);
        }

        auto header_start = ctx.buffer.as_rslice().skip(2);

        uint32_t actual_crc = 0;

        BigEndian::read(
            header_start,
            ctx.result.destination,
            ctx.result.source,
            ctx.payload_length,
            actual_crc
        );

        // now read and validate the header
        auto expected_crc = CastagnoliCRC32::calc(ctx.buffer.as_rslice().take(consts::link::header_fields_size));

        if (expected_crc != actual_crc)
        {
            ctx.reporter->on_bad_header_crc(expected_crc, actual_crc);

            auto header = ctx.buffer.as_rslice().take(consts::link::header_total_size).skip(2);

            // reprocess all header bytes except for the synchronization bytes.
            //
            // Since this segment has length (link_header_total_size - 2) we're
            // guaranteed it'll all be processed from wait_sync1
            return parse_many(State::wait_sync1(), ctx, header);
        }

        if (ctx.payload_length > ctx.max_payload_length)
        {
            ctx.reporter->on_bad_body_length(ctx.max_payload_length, ctx.payload_length);
            return State::wait_sync1();
        }

        return State::wait_body(new_num_buffered);
    }

    LinkParser::State LinkParser::parse_body(const State& state, Context& ctx, seq32_t& input)
    {
        const uint32_t total_frame_size = consts::link::header_total_size + ctx.payload_length + consts::link::crc_size;

        const auto new_num_buffered = transfer_data(state, ctx, input, total_frame_size);

        if (new_num_buffered != total_frame_size)
        {
            return State::wait_body(new_num_buffered);
        }

        ctx.result.payload = ctx.buffer.as_rslice().skip(consts::link::header_total_size).take(ctx.payload_length);
        const auto expected_crc = CastagnoliCRC32::calc(ctx.result.payload);
        auto crcb_start = ctx.buffer.as_rslice().skip(consts::link::header_total_size + ctx.payload_length);

        uint32_t actual_crc = 0;
        UInt32::read_from(crcb_start, actual_crc);

        if (expected_crc != actual_crc)
        {
            ctx.reporter->on_bad_body_crc(expected_crc, actual_crc);
            return State::wait_sync1();
        }

        return State::wait_read(new_num_buffered);
    }

    uint32_t LinkParser::transfer_data(const State& state, Context& ctx, seq32_t& input, uint32_t max_bytes_to_buffer)
    {
        const auto remaining = max_bytes_to_buffer - state.num_buffered;
        const auto num_to_copy = min<uint32_t>(remaining, input.length());
        auto dest = ctx.buffer.as_wslice().skip(state.num_buffered);

        dest.move_from(input.take(num_to_copy));
        input.advance(num_to_copy);

        return num_to_copy + state.num_buffered;
    }
}


