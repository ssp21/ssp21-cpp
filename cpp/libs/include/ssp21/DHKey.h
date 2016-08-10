
#ifndef SSP21_DHKEY_H
#define SSP21_DHKEY_H

#include "SecureBuffer.h"
#include "Constants.h"

namespace ssp21
{		
	class DHKey
	{
		public:

			DHKey();

			openpal::RSlice GetKey();


			openpal::WSlice GetWriteDest();
			void SetLength(uint32_t length);

		private:

			uint32_t m_length;
			SecureBuffer<consts::MAX_DH_KEY_SIZE> m_buffer;
	};
}

#endif
