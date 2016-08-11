
#include "ssp21/Crypto.h"

#include <assert.h>

namespace ssp21
{					
	zero_memory_func_t Crypto::zero_memory_(nullptr);
	hash_func_t Crypto::hash_sha256_(nullptr);
	hmac_func_t Crypto::hmac_sha256_(nullptr);
	gen_keypair_func_t Crypto::gen_keypair_x25519_(nullptr);
	dh_func_t Crypto::dh_x25519_(nullptr);


	void Crypto::zero_memory(openpal::WSlice& buff)
	{
		assert(zero_memory_);
		zero_memory_(buff);
	}

	openpal::RSlice Crypto::calc_hash_sha256(
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(hash_sha256_);
		assert(dest.Size() >= consts::SHA256_HASH_OUTPUT_LENGTH);
		
		return hash_sha256_(data, dest);
	}

	openpal::RSlice Crypto::calc_hmac_sha256(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(hmac_sha256_);
		assert(dest.Size() >= consts::SHA256_HASH_OUTPUT_LENGTH);
		
		return hmac_sha256_(key, data, dest);
	}

	void Crypto::gen_keypair_x25519(KeyPair& pair)
	{
		assert(gen_keypair_x25519_);

		gen_keypair_x25519_(pair);
	}

	void Crypto::dh_x25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec)
	{
		assert(dh_x25519_);
		assert(pub_key.Size() == consts::X25519_KEY_LENGTH);		
		
		dh_x25519_(priv_key, pub_key, output, ec);
	}

	void Crypto::inititalize(
		zero_memory_func_t zero_memory,
		hash_func_t hash_sha256,
		hmac_func_t hmac_sha256,
		gen_keypair_func_t gen_keypair_x25519,
		dh_func_t dh_x25519)
	{
		zero_memory_ = zero_memory,
		hash_sha256_ = hash_sha256;
		hmac_sha256_ = hmac_sha256;
		gen_keypair_x25519_ = gen_keypair_x25519;
		dh_x25519_ = dh_x25519;
	}


}


