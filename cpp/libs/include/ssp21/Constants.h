#ifndef SSP21_CONSTANTS_H
#define SSP21_CONSTANTS_H

#include <cstdint>

namespace ssp21
{
	namespace consts
	{
		const uint8_t SHA256_HASH_OUTPUT_LENGTH = 32;
		const uint8_t X25519_KEY_LENGTH = 32;

		// maximum sizes across algorithm types
		// usually used to size static buffers so that they can be used for any algorithm in that class
		const uint8_t MAX_KEY_SIZE = X25519_KEY_LENGTH;
		const uint8_t MAX_HASH_OUTPUT_SIZE = SHA256_HASH_OUTPUT_LENGTH;
	}
}

#endif
