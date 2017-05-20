
#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <assert.h>

namespace ssp21
{
	bool Crypto::initialized(false);

    void Crypto::zero_memory(const wseq32_t& data)
    {
		assert(initialized);
		zero_memory_impl(data);
    }

    bool Crypto::secure_equals(const seq8_t& lhs, const seq8_t& rhs)
    {
		assert(initialized);
		return secure_equals_impl(lhs, rhs);
    }

    void Crypto::hash_sha256(
        std::initializer_list<seq32_t> data,
        SecureBuffer& output)
    {
		assert(initialized);
		hash_sha256_impl(data, output);
    }

    void Crypto::hmac_sha256(
        const seq8_t& key,
        std::initializer_list<seq32_t> data,
        SecureBuffer& output)
    {
		assert(initialized);
		hmac_sha256_impl(key, data, output);
    }

    void Crypto::gen_keypair_x25519(KeyPair& pair)
    {
		assert(initialized);
		gen_keypair_x25519_impl(pair);
    }

    void Crypto::dh_x25519(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec)
    {
		assert(initialized);
        
        if ((priv_key.get_type() != BufferType::x25519_key) || (pub_key.length() != consts::crypto::x25519_key_length))
        {
            ec = CryptoError::bad_key_type;
            return;
        }

		dh_x25519_impl(priv_key, pub_key, output, ec);
    }

    void Crypto::hkdf_sha256(
        const seq8_t& salt,
        std::initializer_list<seq32_t> input_key_material,
        SymmetricKey& output1,
        SymmetricKey& output2)
    {
		assert(initialized);
		hkdf_sha256_impl(salt, input_key_material, output1, output2);
    }

	void Crypto::gen_keypair_ed25519(KeyPair& pair)
	{
		assert(initialized);
		gen_keypair_ed25519_impl(pair);
	}

    bool Crypto::initialize()
    {		
		assert(!initialized);
		initialized = initialize_impl();
		return initialized;
    }


}


