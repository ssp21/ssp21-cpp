#include "VLength.h"

#include "openpal/serialization/BigEndian.h"

using namespace openpal;

namespace ssp21
{

    FormatError VLength::write(uint32_t value, wseq32_t& dest)
    {
        while(true)
        {
            const auto more = value >> 7;
            const auto current = (value & 0x7F) | (more ? 0x80 : 0x00);

            if (!UInt8::write_to(dest, current))
            {
                return FormatError::insufficient_space;
            }

            if (!more) return FormatError::ok;

            value = more;
        }
    }

    ParseError VLength::read(uint32_t& value, seq32_t& src)
    {
        uint8_t byte = 0;

        while (true)
        {
            uint8_t current;
            if (!UInt8::read_from(src, current)) return ParseError::insufficient_bytes;

            if (byte == 4 && (current > 0x0F)) return ParseError::impl_capacity_limit; // TODO

            const bool more = (current & top_bit) != 0;

            value |= (current & bottom_bits);

            if (!more)
            {
                return ParseError::ok;
            }

            // prepare for next iteration
            value <<= 7;
            ++byte;
        }
    }

    size_t VLength::size_in_bytes(uint32_t value)
    {
        if (value <= max_1_byte_value)
        {
            return 1;
        }
        else if (value <= max_2_byte_value)
        {
            return 2;
        }
        else if (value <= max_3_byte_value)
        {
            return 3;
        }
        else if (value <= max_4_byte_value)
        {
            return 4;
        }
        else
        {
            return 5;
        }
    }


}


