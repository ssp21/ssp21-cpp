
#include "ssp21/crypto/MessageParser.h"

#include "openpal/serialization/BigEndian.h"

using namespace openpal;

namespace ssp21
{

    template <class T>
    ParseError read_integer(RSlice& input, typename T::type_t& value)
    {
        return T::read_from(input, value) ? ParseError::ok : ParseError::insufficient_bytes;
    }

    template <class Spec>
    ParseError read_enum(RSlice& input, typename Spec::enum_type_t& value)
    {
        if (input.length() < 1)
        {
            return ParseError::insufficient_bytes;
        }
        else
        {
            value = Spec::from_type(input[0]);
            input.advance(1);

            if (value == Spec::enum_type_t::undefined)
            {
                return ParseError::undefined_enum;
            }

            return ParseError::ok;
        }
    }   

    ParseError MessageParser::read(openpal::RSlice& input, uint8_t& value)
    {
        return read_integer<UInt8>(input, value);
    }

    ParseError MessageParser::read(RSlice& input, uint16_t& value)
    {
        return read_integer<UInt16>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, uint32_t& value)
    {
        return read_integer<UInt32>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, Function& value)
    {
        return read_enum<FunctionSpec>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, CertificateMode& value)
    {
        return read_enum<CertificateModeSpec>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, DHMode& value)
    {
        return read_enum<DHModeSpec>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, HandshakeError& value)
    {
        return read_enum<HandshakeErrorSpec>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, NonceMode& value)
    {
        return read_enum<NonceModeSpec>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, SessionMode& value)
    {
        return read_enum<SessionModeSpec>(input, value);
    }

    ParseError MessageParser::read(openpal::RSlice& input, HashMode& value)
    {
        return read_enum<HashModeSpec>(input, value);
    }
	
}

