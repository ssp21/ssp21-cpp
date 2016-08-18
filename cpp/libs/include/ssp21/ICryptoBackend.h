
#ifndef SSP21_ICRYTPTOBACKEND_H
#define SSP21_ICRYTPTOBACKEND_H

#include "BufferTypes.h"

#include <initializer_list>
#include <system_error>

namespace ssp21
{				
	class ICryptoBackend
	{

	public:

		virtual void zero_memory(openpal::WSlice data) = 0;

		virtual bool secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs) = 0;

		virtual void hash_sha256(std::initializer_list<openpal::RSlice> data, SecureBuffer &output) = 0;

		virtual void hmac_sha256(const openpal::RSlice &key, std::initializer_list<openpal::RSlice> data, SecureBuffer &output) = 0;

		virtual void gen_keypair_x25519(KeyPair& pair) = 0;

		virtual void dh_x25519(const PrivateKey& priv_key, const openpal::RSlice& pub_key, DHOutput& output, std::error_code& ec) = 0;		
	};	
}

#endif
