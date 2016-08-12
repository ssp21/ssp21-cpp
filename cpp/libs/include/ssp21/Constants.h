#ifndef SSP21_CONSTANTS_H
#define SSP21_CONSTANTS_H

#include <cstdint>

namespace ssp21
{
	namespace consts
	{
		const uint8_t sha256_hash_output_length = 32;
		const uint8_t x25519_key_length = 32;

		// maximum sizes across algorithm types
		// usually used to size static buffers so that they can be used for any algorithm in that class
		const uint8_t max_key_length = x25519_key_length;
		const uint8_t max_hash_output_length = sha256_hash_output_length;
	}
}

#endif
