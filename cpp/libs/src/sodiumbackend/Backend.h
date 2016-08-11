

#ifndef SODIUMBACKEND_BACKEND_H
#define SODIUMBACKEND_BACKEND_H

#include "ssp21/CryptoBackend.h"

namespace ssp21
{	
	namespace sodium
	{
		void zero_memory(openpal::WSlice& buff);

		bool secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs);

		void calc_hash_sha256(std::initializer_list<openpal::RSlice> data, HashOutput& output);

		void calc_hmac_sha256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, HashOutput& output);

		void gen_keypair_x25519(KeyPair& pair);

		void dh_x25519(const Key& priv_key, const Key& pub_key, Key& output, std::error_code& ec);
	}
}

#endif
