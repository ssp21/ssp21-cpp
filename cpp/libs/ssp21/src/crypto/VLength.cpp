#include "ssp21/crypto/VLength.h"

#include "ser4cpp/serialization/BigEndian.h"

namespace ssp21
{

    FormatError VLength::write(uint32_t value, wseq32_t& dest)
    {
        while(true)
        {
            const auto more = value >> 7;
            const auto current = (value & 0x7F) | (more ? 0x80 : 0x00);

            if (!ser4cpp::UInt8::write_to(dest, current))
            {
                return FormatError::insufficient_space;
            }

            if (!more) return FormatError::ok;

            value = more;
        }
    }

    ParseError VLength::read(uint32_t& value, seq32_t& src)
    {
        value = 0;
        uint8_t byte = 0;

        while (true)
        {
            uint8_t current;
            if (!ser4cpp::UInt8::read_from(src, current)) return ParseError::insufficient_bytes;

            if (byte == 4 && (current > 0x0F)) return ParseError::bad_vlength;

            const bool more = (current & top_bit_mask) != 0;
            const auto bottom_bits = current & bottom_bits_mask;

            value |= static_cast<uint32_t>(bottom_bits) << (7 * byte);

            if (!more)
            {
                // the last byte can't be zero if it's not the first byte
                // this would indicate that the value could have been encoded
                // in fewer bytes
                if (byte != 0 && bottom_bits == 0) return ParseError::bad_vlength;

                return ParseError::ok;
            }

            // prepare for next iteration
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


