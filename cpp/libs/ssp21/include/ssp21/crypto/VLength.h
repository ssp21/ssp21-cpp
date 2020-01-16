#ifndef SSP21_SEQUENCELENGTH_H
#define SSP21_SEQUENCELENGTH_H

#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/util/SequenceTypes.h"

namespace ssp21 {
/**
    *
    *	Variable-length unsigned 32-bit integer encoding/decoding ala ASN.1 DER
	*
	*   The first byte may represent a number in the interval [0,127] OR if the top
	*   bit is set, the lower 7 bytes encode the count of bytes that follow.
	*	
	*   Values SHALL be encoded in the fewest bytes possible, and parsers SHALL reject
	*   such invalid encodings.
	*
    *   num bytes  |  min         |  max          |  Notes
    *   __________________________________________|_______________________________
    *		1	   |   0          |  127          |  Only if top bit is NOT set
    *		2	   |   128        |  255          |  First byte == 0x81
    *       3	   |   256        |  65535        |  First byte == 0x82
    *       4	   |   65536      |  16777215     |  First byte == 0x83
    *       5      |   16777216   |  4294967295   |  First byte == 0x84
	*
    */
struct VLength : private ser4cpp::StaticOnly {
    static FormatError write(uint32_t value, wseq32_t& dest);

    static ParseError read(uint32_t& value, seq32_t& src);

    static uint8_t num_values_bytes(uint32_t value);

    static size_t size(uint32_t value)
    {
        return static_cast<size_t>(num_values_bytes(value)) + 1;
    }
};

}

#endif
