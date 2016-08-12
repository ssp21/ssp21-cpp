
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

	void Crypto::dh_x25519(const PrivateKey& priv_key, const PublicKey& pub_key, DHOutput& output, std::error_code& ec)
	{
		assert(backend_);

		// TODO: make error codes for these since user has to check error anyway
		assert(priv_key.get_type() == BufferType::X25519_KEY);
		assert(pub_key.get_type() == BufferType::X25519_KEY);
				
		backend_->dh_x25519(priv_key, pub_key, output, ec);
	}

	void Crypto::inititalize(ICryptoBackend& backend)
	{
		backend_ = &backend;
	}


}


