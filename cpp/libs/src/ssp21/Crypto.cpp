
#include "ssp21/Crypto.h"

#include <assert.h>

namespace ssp21
{					
	CryptoBackend Crypto::backend_;

	void Crypto::zero_memory(openpal::WSlice& buff)
	{
		assert(backend_.zero_memory);
		backend_.zero_memory(buff);
	}

	bool Crypto::secure_compare(const openpal::RSlice& lhs, const openpal::RSlice& rhs)
	{
		assert(backend_.secure_compare);
		return backend_.secure_compare(lhs, rhs);
	}

	openpal::RSlice Crypto::calc_hash_sha256(
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(backend_.hash_sha256);
		assert(dest.Size() >= consts::SHA256_HASH_OUTPUT_LENGTH);
		
		return backend_.hash_sha256(data, dest);
	}

	openpal::RSlice Crypto::calc_hmac_sha256(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest)
	{
		assert(backend_.hmac_sha256);
		assert(dest.Size() >= consts::SHA256_HASH_OUTPUT_LENGTH);
		
		return backend_.hmac_sha256(key, data, dest);
	}

	void Crypto::gen_keypair_x25519(KeyPair& pair)
	{
		assert(backend_.gen_keypair_x25519);
		backend_.gen_keypair_x25519(pair);
	}

	void Crypto::dh_x25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec)
	{
		assert(backend_.dh_x25519);
		assert(pub_key.Size() == consts::X25519_KEY_LENGTH);		
		
		backend_.dh_x25519(priv_key, pub_key, output, ec);
	}

	void Crypto::inititalize(const CryptoBackend& backend)
	{
		backend_ = backend;
	}


}


