#ifndef SSP21_MESSAGE_PARSER_H
#define SSP21_MESSAGE_PARSER_H

#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/gen/ParseError.h"

#include "openpal/container/RSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class MessageParser : private openpal::StaticOnly
    {

    public:

        /// Enforces that the first byte is the expected function and expects all data to be consumed.
        template <typename ReadFields>
        static ParseError read_message(const openpal::RSlice& input, Function expected, const ReadFields& read_fields)
        {
            openpal::RSlice copy(input);

            EnumField<FunctionSpec> func;
            auto err = func.read(copy);
            if (any(err)) return err;
            if (func != expected) return ParseError::unexpected_function;

            err = read_fields(copy);
            if (any(err)) return err;

            // top level messages must always fully read the input
            return copy.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
        }

        template <typename T, typename... Args>
        static ParseError read_fields(openpal::RSlice& input, T& value, Args& ... args)
        {
			auto err = value.read(input);
            if (any(err)) return err;
            return read_fields(input, args...);
        }

        static ParseError read_fields(openpal::RSlice&)
        {
            return ParseError::ok;
        }
    };
}

#endif