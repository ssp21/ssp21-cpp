
#include "ssp21/Crypto.h"

#include "ssp21/ErrorCodes.h"

#include <assert.h>

namespace ssp21
{				
	ICryptoBackend* Crypto::backend_(nullptr);
	
	void Crypto::zero_memory(openpal::WSlice data)
	{
		assert(backend_);
		backend_->zero_memory(data);
	}

	bool Crypto::secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)
	{
		assert(backend_);
		return backend_->secure_equals(lhs, rhs);
	}

	void Crypto::hash_sha256(
            std::initializer_list<openpal::RSlice> data,
			SymmetricKey& output)
	{
		assert(backend_);
		backend_->hash_sha256(data, output);
	}

	void Crypto::hmac_sha256(
			const openpal::RSlice &key,
			std::initializer_list<openpal::RSlice> data,
			SymmetricKey& output)
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

		if ((priv_key.get_type() != BufferType::X25519_KEY) || (pub_key.get_type() != BufferType::X25519_KEY))
		{
			ec = Error::BAD_KEY_TYPE;
			return;
		}

		backend_->dh_x25519(priv_key, pub_key, output, ec);
	}

	void Crypto::hkdf_sha256(
		const openpal::RSlice &chaining_key,
		std::initializer_list<openpal::RSlice> input_key_material,
		SymmetricKey& output1,
		SymmetricKey& output2)
	{
		hkdf(&hmac_sha256, chaining_key, input_key_material, output1, output2);
	}	

	void Crypto::hkdf(
		hmac_func_t hmac,
		const openpal::RSlice &chaining_key,
		std::initializer_list<openpal::RSlice> input_key_material,
		SymmetricKey& output1,
		SymmetricKey& output2)
	{
		// extract
		SymmetricKey temp_key;
		hmac(chaining_key, input_key_material, temp_key);

		const uint8_t ONE = 0x01;
		const uint8_t TWO = 0x02;
		
		// expand
		hmac(temp_key.as_slice(), { openpal::RSlice(&ONE, 1) }, output1);
		hmac(temp_key.as_slice(), { output1.as_slice(), openpal::RSlice(&TWO, 1) }, output2);
		
		// this will truncate the lengths in the event that the hmac-output size is > the symmetric key size we need
		// TODO: research how noise implementations handle this
		output1.set_type(BufferType::SYMMETRIC_KEY);
		output2.set_type(BufferType::SYMMETRIC_KEY);
	}	

	void Crypto::inititalize(ICryptoBackend& backend)
	{
		backend_ = &backend;
	}


}


