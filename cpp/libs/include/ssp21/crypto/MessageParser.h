#ifndef SSP21_MESSAGE_PARSER_H
#define SSP21_MESSAGE_PARSER_H

#include "ssp21/crypto/gen/ParseError.h"

#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/gen/CertificateMode.h"
#include "ssp21/crypto/gen/DHMode.h"
#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/SessionMode.h"
#include "ssp21/crypto/gen/HashMode.h"

#include "ssp21/crypto/SequenceTypes.h"
#include "ssp21/crypto/IMessageField.h"

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

            Function func;
            auto err = read(copy, func);
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
            auto err = read(input, value);
            if (any(err)) return err;
            return read_fields(input, args...);
        }

        static ParseError read_fields(openpal::RSlice&)
        {
            return ParseError::ok;
        }

        // integers
        static ParseError read(openpal::RSlice& input, uint8_t& value);
        static ParseError read(openpal::RSlice& input, uint16_t& value);
        static ParseError read(openpal::RSlice& input, uint32_t& value);

        // enums
        static ParseError read(openpal::RSlice& input, Function& value);
        static ParseError read(openpal::RSlice& input, CertificateMode& value);
        static ParseError read(openpal::RSlice& input, DHMode& value);
        static ParseError read(openpal::RSlice& input, HandshakeError& value);
        static ParseError read(openpal::RSlice& input, NonceMode& value);
        static ParseError read(openpal::RSlice& input, SessionMode& value);
        static ParseError read(openpal::RSlice& input, HashMode& value);

        // any field
        static ParseError read(openpal::RSlice& input, IMessageField& field)
        {
            return field.read(input);
        }
    };
}

#endif