

#ifndef SODIUMBACKEND_BACKEND_H
#define SODIUMBACKEND_BACKEND_H

#include <openpal/container/RSlice.h>
#include <openpal/container/WSlice.h>

#include <initializer_list>

#include "ssp21\Crypto.h"

namespace ssp21
{	
	namespace sodium
	{
		void Zero(openpal::WSlice& buff);

		openpal::RSlice SHA256(std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest);

		openpal::RSlice HMAC_SHA256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, openpal::WSlice& dest);

		void GenKeyPair_X25519(KeyPair& pair);

		void DH_X25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec);
	}
}

#endif