#ifndef SSP21_CONSTANTS_H
#define SSP21_CONSTANTS_H

#include <cstdint>

namespace ssp21
{
    namespace consts
    {
		const uint16_t ssp21_protocol_version = 0x0000;

        // implementation constants
        const uint8_t max_certificate_chain = 3;
        const uint8_t max_seq_of_seq = max_certificate_chain;

        // cryptographic constants
        const uint8_t sha256_hash_output_length = 32;
        const uint8_t x25519_key_length = 32;
        const uint8_t symmetric_ley_length = 32;

        // maximum length_ required buffer_ length_ across algorithm types
        const uint8_t max_primitive_buffer_length = x25519_key_length;
    }
}

#endif
