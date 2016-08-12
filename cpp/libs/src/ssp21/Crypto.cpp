
#include "ssp21/Crypto.h"

#include <assert.h>

namespace ssp21
{				
	ICryptoBackend* Crypto::backend_(nullptr);

	
	void Crypto::zero_memory(openpal::WSlice& buff)
	{
		assert(backend_);
		backend_->zero_memory(buff);
	}

	bool Crypto::secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)
	{
		assert(backend_);
		return backend_->secure_equals(lhs, rhs);
	}

	void Crypto::hash_sha256(
            std::initializer_list<openpal::RSlice> data,
            HashOutput &output)
	{
		assert(backend_);
		backend_->hash_sha256(data, output);
	}

	void Crypto::hmac_sha256(
			const openpal::RSlice &key,
			std::initializer_list<openpal::RSlice> data,
			HashOutput &output)
	{
		assert(backend_);
		backend_->hmac_sha256(key, data, output);
	}

	void Crypto::gen_keypair_x25519(KeyPair& pair)
	{
		assert(backend_);
		backend_->gen_keypair_x25519(pair);
	}

	void Crypto::dh_x25519(const Key& priv_key, const Key& pub_key, Key& output, std::error_code& ec)
	{
		assert(backend_);
		assert(priv_key.get_key_type() == KeyType::X25519);
		assert(pub_key.get_key_type() == KeyType::X25519);
				
		backend_->dh_x25519(priv_key, pub_key, output, ec);
	}

	void Crypto::inititalize(ICryptoBackend& backend)
	{
		backend_ = &backend;
	}


}


