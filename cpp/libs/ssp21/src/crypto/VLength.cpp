#include "ssp21/crypto/VLength.h"

#include "ser4cpp/serialization/BigEndian.h"

using namespace ser4cpp;

namespace ssp21 {
const uint32_t max_one_byte_value = 127;
const uint32_t max_four_byte_value = 16777215; // 2^24 - 1
const uint8_t top_bit_mask = 0x80;
const uint8_t bottom_bits_mask = 0x7F;

FormatError VLength::write(uint32_t value, wseq32_t& dest)
{
    const auto num_value_bytes = num_values_bytes(value);

    if (num_value_bytes == 0) {
        return dest.put(value) ? FormatError::ok : FormatError::insufficient_space;
    }

    // encode the count byte
    if (!dest.put(top_bit_mask | num_value_bytes)) {
        return FormatError::insufficient_space;
    }

    switch (num_value_bytes) {
    case (1):
        return dest.put(value) ? FormatError::ok : FormatError::insufficient_space;
    case (2):
        return UInt16::write_to(dest, static_cast<uint16_t>(value)) ? FormatError::ok : FormatError::insufficient_space;
    case (3): {
        const auto top_byte = static_cast<uint8_t>((value >> 16) & 0xFF);
        const auto bottom_2_bytes = static_cast<uint16_t>(value & 0xFFFF);
        if (!dest.put(top_byte)) {
            return FormatError::insufficient_space;
        }
        return UInt16::write_to(dest, bottom_2_bytes) ? FormatError::ok : FormatError::insufficient_space;
    }
    default: // must be 4 bytes
        return UInt32::write_to(dest, value) ? FormatError::ok : FormatError::insufficient_space;
    };
}

ParseError VLength::read(uint32_t& value, seq32_t& src)
{
    uint8_t top_byte = 0;
    if (!UInt8::read_from(src, top_byte)) {
        return ParseError::insufficient_bytes;
    }

    // single byte encoding
    if ((top_byte & top_bit_mask) == 0) {
        value = top_byte;
        return ParseError::ok;
    }

    const uint8_t count_of_bytes = top_byte & bottom_bits_mask;

    if (count_of_bytes < 1 || count_of_bytes > 4) {
        return ParseError::bad_vlength;
    }

    uint32_t ret = 0;
    for (auto i = 0; i < count_of_bytes; ++i) {
        ret <<= 8;
        uint8_t byte = 0;
        if (!UInt8::read_from(src, byte)) {
            return ParseError::insufficient_bytes;
        }
        ret |= byte;
    }

    if (num_values_bytes(ret) != count_of_bytes) {
        return ParseError::bad_vlength;
    }

    value = ret;
    return ParseError::ok;
}

uint8_t VLength::num_values_bytes(uint32_t value)
{
    if (value <= max_one_byte_value) {
        return 0; // value encoded in first byte
    } else if (value <= std::numeric_limits<uint8_t>::max()) {
        return 1;
    } else if (value <= std::numeric_limits<uint16_t>::max()) {
        return 2;
    } else if (value <= max_four_byte_value) {
        return 3;
    } else {
        return 4;
    }
}

}
