
#ifndef SSP21_KEY_H
#define SSP21_KEY_H

#include "Constants.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/container/StaticBuffer.h>

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

			virtual ~Key() {}

			openpal::RSlice as_slice() const;
			
			KeyType get_key_type() const;

			openpal::WSlice get_write_slice();
			
			void set_key_type(KeyType key_type);			

		protected:

			Key();

		private:	

			static uint32_t get_key_length(KeyType);

			uint32_t length_;
			KeyType key_type_;

		protected:
			openpal::StaticBuffer<consts::max_key_length> buffer_;
	};

	// A secure key zeros its memory upon destruction
	class SecureKey : public Key
	{
	public:
		virtual ~SecureKey();
	};

	// specialized types that actually get used

	class PublicKey final : public Key {};

	class PrivateKey final : public SecureKey {};
	
	class DHOutput final : public SecureKey {};	

	struct KeyPair final
	{
		PublicKey public_key;
		PrivateKey private_key;
	};
}

#endif
