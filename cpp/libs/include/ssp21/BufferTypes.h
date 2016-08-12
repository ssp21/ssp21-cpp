
#ifndef SSP21_BUFFER_TYPES_H
#define SSP21_BUFFER_TYPES_H

#include "Constants.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/container/StaticBuffer.h>

namespace ssp21
{		
	// the buffer type also defines the length
	enum class BufferType
	{
		EMPTY,
		X25519_KEY,
		SHA256
	};

	class Buffer : private openpal::Uncopyable
	{
		public:	

			virtual ~Buffer() {}

			openpal::RSlice as_slice() const;
			
			BufferType get_type() const;

			openpal::WSlice get_write_slice();
			
			void set_type(BufferType key_type);

		protected:

			Buffer();

		private:	

			static uint32_t get_buffer_length(BufferType);

			uint32_t length_;
			BufferType buffer_type_;

		protected:
			openpal::StaticBuffer<consts::max_primitive_buffer_length> buffer_;
	};

	// A secure key zeros its buffer upon destruction
	class SecureBuffer : public Buffer
	{
	public:
		virtual ~SecureBuffer();
	};

	// specialized types that actually get used

	class PublicKey final : public Buffer {};

	class PrivateKey final : public SecureBuffer {};
	
	class DHOutput final : public SecureBuffer {};

	class HashOutput final : public SecureBuffer {};

	struct KeyPair final
	{
		PublicKey public_key;
		PrivateKey private_key;
	};
}

#endif
