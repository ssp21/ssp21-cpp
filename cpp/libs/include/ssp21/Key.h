
#ifndef SSP21_KEY_H
#define SSP21_KEY_H

#include "SecureBuffer.h"
#include "Constants.h"

namespace ssp21
{		
	class Key
	{
		public:

			Key();

			openpal::RSlice as_slice();

			openpal::WSlice get_write_slice();

			// TODO - possibly use an enumeration here of possible key types?
			void set_length(uint32_t length);

		private:

			uint32_t length_;
			SecureBuffer<consts::MAX_KEY_SIZE> buffer_;
	};

	struct KeyPair
	{
		Key public_key;
		Key private_key;
	};
}

#endif
