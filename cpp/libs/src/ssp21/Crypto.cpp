
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

	bool Crypto::secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)
	{
		assert(backend_.secure_compare);
		return backend_.secure_compare(lhs, rhs);
	}

	void Crypto::calc_hash_sha256(
		std::initializer_list<openpal::RSlice> data,
		HashOutput& output)
	{
		assert(backend_.hash_sha256);				
		backend_.hash_sha256(data, output);
	}

	void Crypto::calc_hmac_sha256(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		HashOutput& output)
	{
		assert(backend_.hmac_sha256);		
		backend_.hmac_sha256(key, data, output);
	}

	void Crypto::gen_keypair_x25519(KeyPair& pair)
	{
		assert(backend_.gen_keypair_x25519);
		backend_.gen_keypair_x25519(pair);
	}

	void Crypto::dh_x25519(const Key& priv_key, const Key& pub_key, Key& output, std::error_code& ec)
	{
		assert(backend_.dh_x25519);
		assert(priv_key.get_key_type() == KeyType::X25519);
		assert(pub_key.get_key_type() == KeyType::X25519);
				
		backend_.dh_x25519(priv_key, pub_key, output, ec);
	}

	void Crypto::inititalize(const CryptoBackend& backend)
	{
		backend_ = backend;
	}


}


