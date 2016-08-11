

#ifndef SODIUMBACKEND_BACKEND_H
#define SODIUMBACKEND_BACKEND_H

#include <openpal/container/RSlice.h>
#include <openpal/container/WSlice.h>

#include <initializer_list>

#include "ssp21/Crypto.h"

namespace ssp21
{	
	namespace sodium
	{
		void zero_memory(openpal::WSlice& buff);

		bool secure_compare(const openpal::RSlice& lhs, const openpal::RSlice& rhs);

		openpal::RSlice calc_hash_sha256(std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest);

		openpal::RSlice calc_hmac_sha256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest);

		void gen_keypair_x25519(KeyPair& pair);

		void dh_x25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec);
	}
}

#endif
