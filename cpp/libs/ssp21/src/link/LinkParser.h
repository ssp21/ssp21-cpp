
#ifndef SSP21_LINKPARSER_H
#define SSP21_LINKPARSER_H

#include "ser4cpp/container/Buffer.h"
#include "ser4cpp/util/Uncopyable.h"

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/link/Addresses.h"

namespace ssp21 {
class LinkParser : ser4cpp::Uncopyable {

public:
    class IReporter {
    public:
        virtual void on_bad_header_crc(uint32_t expected, uint32_t actual) = 0;
        virtual void on_bad_body_crc(uint32_t expected, uint32_t actual) = 0;
        virtual void on_bad_body_length(uint32_t max_allowed, uint32_t actual) = 0;
    };

    struct Result : public Addresses {
        seq32_t payload;
    };

public:
    LinkParser(uint16_t max_payload_length, IReporter& reporter);

    void reset();

    bool parse(seq32_t& input);

    bool read(Result& result) const
    {
        if (!this->state.is_wait_read()) {
            return false;
        }

        result = this->context.result;
        return true;
    }

    bool has_result() const
    {
        return this->state.is_wait_read();
    }

private:
    class State {

    public:
        enum class Value {
            wait_sync1,
            wait_sync2,
            wait_header,
            wait_body,
            wait_read
        };

        State() {}

        static State wait_sync1()
        {
            return State(Value::wait_sync1, 0);
        }
        static State wait_sync2()
        {
            return State(Value::wait_sync2, 1);
        }
        static State wait_header(uint32_t num)
        {
            return State(Value::wait_header, num);
        }
        static State wait_body(uint32_t num)
        {
            return State(Value::wait_body, num);
        }
        static State wait_read(uint32_t num)
        {
            return State(Value::wait_read, num);
        }

        Value value = Value::wait_sync1;
        uint32_t num_buffered = 0;

        bool is_wait_read() const
        {
            return value == Value::wait_read;
        }

    private:
        State(Value state, uint32_t num_buffered)
            : value(state)
            , num_buffered(num_buffered)
        {
        }
    };

    struct Context {
        Context(uint16_t max_payload_length, IReporter& reporter);

        const uint16_t max_payload_length;
        IReporter* reporter;
        ser4cpp::Buffer buffer;

        Result result;
        uint16_t payload_length = 0;
    };

    static State parse_one(const State& state, Context& ctx, seq32_t& input);
    static State parse_many(const State& state, Context& ctx, seq32_t& input);

    static State parse_sync1(const State& state, Context& ctx, seq32_t& input);
    static State parse_sync2(const State& state, Context& ctx, seq32_t& input);
    static State parse_header(const State& state, Context& ctx, seq32_t& input);
    static State parse_body(const State& state, Context& ctx, seq32_t& input);

    // helpers
    static uint32_t transfer_data(const State& state, Context& ctx, seq32_t& input, uint32_t max);

    LinkParser() = delete;

    Context context;

    State state = State::wait_sync1();
};

}

#endif
