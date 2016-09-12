
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

}

