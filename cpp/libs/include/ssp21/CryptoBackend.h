
#ifndef SSP21_CRYTPTOBACKEND_H
#define SSP21_CRYTPTOBACKEND_H

#include "Key.h"

#include <initializer_list>
#include <system_error>

namespace ssp21
{			
	typedef void (*zero_memory_func_t)(openpal::WSlice&);

	typedef bool (*secure_compare_func_t)(const openpal::RSlice& lhs, const openpal::RSlice& rhs);

	typedef openpal::RSlice (*hash_func_t)(		
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest
	);

	typedef openpal::RSlice (*hmac_func_t)(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest
	);
	
	typedef void (*gen_keypair_func_t)(KeyPair& pair);

	typedef void (*dh_func_t)(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec);

	struct CryptoBackend
	{
		CryptoBackend() : 
			zero_memory(nullptr),
			secure_compare(nullptr),
			hash_sha256(nullptr),
			hmac_sha256(nullptr),
			gen_keypair_x25519(nullptr),
			dh_x25519(nullptr)
		{}

		CryptoBackend(
				zero_memory_func_t zero_memory,
				secure_compare_func_t secure_compare,
				hash_func_t hash_sha256,
				hmac_func_t hmac_sha256,
				gen_keypair_func_t gen_keypair_x25519,
				dh_func_t dh_x25519) :
			zero_memory(zero_memory),
			secure_compare(secure_compare),
			hash_sha256(hash_sha256),
			hmac_sha256(hmac_sha256),
			gen_keypair_x25519(gen_keypair_x25519),
			dh_x25519(dh_x25519)
		{}

		zero_memory_func_t zero_memory;
		secure_compare_func_t secure_compare;
		hash_func_t hash_sha256;
		hmac_func_t hmac_sha256;
		gen_keypair_func_t gen_keypair_x25519;
		dh_func_t dh_x25519;
	};	
}

#endif
