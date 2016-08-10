
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

			openpal::RSlice GetKey();

			openpal::WSlice GetWriteDest();

			void SetLength(uint32_t length);

		private:

			uint32_t m_length;
			SecureBuffer<consts::MAX_KEY_SIZE> m_buffer;
	};

	class KeyPair
	{

	public:

		Key publicKey;
		Key privateKey;
	};
}

#endif
