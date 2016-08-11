
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include "Key.h"

#include <initializer_list>
#include <system_error>

namespace ssp21
{		
	typedef void (*zero_memory_func_t)(openpal::WSlice&);

	typedef openpal::RSlice (*hash_func_t)(		
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest
	);

	typedef openpal::RSlice(*hmac_func_t)(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest
	);
	
	typedef void(*gen_keypair_func_t)(KeyPair& pair);

	typedef void (*dh_func_t)(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec);

	
	/**
	* Assumming for the time being that a static backend is fine 
	* to keep dependency injection simple.
	*/
	class Crypto
	{

	public:		

		static void zero_memory(openpal::WSlice&);
		
		static openpal::RSlice calc_hash_sha256(
			std::initializer_list<openpal::RSlice> data,
			openpal::WSlice& dest);

		static openpal::RSlice calc_hmac_sha256(
			const openpal::RSlice& key,
			std::initializer_list<openpal::RSlice> data,
			openpal::WSlice& dest);

		static void gen_keypair_x25519(KeyPair& pair);

		static void dh_x25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec);
		
		static void inititalize(
			zero_memory_func_t zero_memory,
			hash_func_t hash_sha256,
			hmac_func_t hmac_sha256,
			gen_keypair_func_t gen_keypair_x25519,
			dh_func_t dh_x25519
		);


	private:

		/// Function typedefs all initialized to nullptr. Concrete backends will initialize these.
		/// These should not be used directly

		static zero_memory_func_t zero_memory_;
		static hash_func_t hash_sha256_;
		static hmac_func_t hmac_sha256_;
		static gen_keypair_func_t gen_keypair_x25519_;
		static dh_func_t dh_x25519_;
		

		Crypto() = delete;
		Crypto(Crypto&) = delete;
	};
}

#endif
