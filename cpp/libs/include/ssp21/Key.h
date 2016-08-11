
#ifndef SSP21_KEY_H
#define SSP21_KEY_H

#include "SecureBuffer.h"
#include "Constants.h"

#include <openpal/util/Uncopyable.h>

namespace ssp21
{		
	enum class KeyType
	{
		EMPTY,
		X25519
	};

	class Key : private openpal::Uncopyable
	{
		public:

			Key();

			openpal::RSlice as_slice() const;
			
			KeyType get_key_type() const;

			openpal::WSlice get_write_slice();
			
			void set_key_type(KeyType key_type);			

		private:	

			static uint32_t get_key_length(KeyType);

			uint32_t length_;
			KeyType key_type_;
			SecureBuffer<consts::MAX_KEY_SIZE> buffer_;
	};

	struct KeyPair
	{
		Key public_key;
		Key private_key;
	};
}

#endif
