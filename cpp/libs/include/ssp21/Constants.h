#ifndef SSP21_CONSTANTS_H
#define SSP21_CONSTANTS_H

#include <cstdint>

namespace ssp21
{
	namespace consts
	{
		const uint8_t sha256_hash_output_length = 32;
		const uint8_t x25519_key_length = 32;

		// maximum size required buffer size across algorithm types		
		const uint8_t max_primitive_buffer_length = x25519_key_length;		
	}
}

#endif
