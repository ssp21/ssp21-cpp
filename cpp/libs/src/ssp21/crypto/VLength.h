#ifndef SSP21_SEQUENCELENGTH_H
#define SSP21_SEQUENCELENGTH_H

#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"

namespace ssp21
{
    /**
    *
    *	Variable-length unsigned 32-bit integer encoding/decoding
    *
    *   num bytes | # bits | max value   |  encoding
    *   ______________________________________________________
    *		1	  |    7    |  127        |  { 0x7F }
    *		2	  |	   14   |  16383      |  { 0xFF, 0x7F }
    *       3	  |    21   |  2097151    |  { 0xFF, 0xFF, 0x7F }
    *       4	  |    28   |  268435455  |  { 0xFF, 0xFF, 0xFF, 0x7F }
    *
    *  We can only use the bottom 4 bits of the 5th byte.
    *
    *  2^32 - 1 encodes to { 0xFF, 0xFF, 0xFF, 0xFF, 0x0F }
    *
    *  Therefore, the 5th byte can NEVER be > than 0x0F
    *
    */
    class VLength : private openpal::StaticOnly
    {
        static const uint8_t top_bit_mask = 0x80;
        static const uint8_t bottom_bits_mask = 0x7F;

        static const uint32_t max_1_byte_value = 127;
        static const uint32_t max_2_byte_value = 16383;
        static const uint32_t max_3_byte_value = 2097151;
        static const uint32_t max_4_byte_value = 268435455;

    public:

        static FormatError write(uint32_t value, wseq32_t& dest);

        static ParseError read(uint32_t& value, seq32_t& src);

        static size_t size_in_bytes(uint32_t value);
    };

}

#endif
