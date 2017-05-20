
#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <assert.h>

namespace ssp21
{
    ICryptoBackend* Crypto::backend_(nullptr);

    void Crypto::zero_memory(const wseq32_t& data)
    {
        assert(backend_);
        backend_->zero_memory(data);
    }

    bool Crypto::secure_equals(const seq8_t& lhs, const seq8_t& rhs)
    {
        assert(backend_);
        return backend_->secure_equals(lhs, rhs);
    }

    void Crypto::hash_sha256(
        std::initializer_list<seq32_t> data,
        SecureBuffer& output)
    {
        assert(backend_);
        backend_->hash_sha256(data, output);
    }

    void Crypto::hmac_sha256(
        const seq8_t& key,
        std::initializer_list<seq32_t> data,
        SecureBuffer& output)
    {
        assert(backend_);
        backend_->hmac_sha256(key, data, output);
    }

    void Crypto::gen_keypair_x25519(KeyPair& pair)
    {
        assert(backend_);
        backend_->gen_keypair_x25519(pair);
    }

    void Crypto::dh_x25519(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec)
    {
        assert(backend_);

        if ((priv_key.get_type() != BufferType::x25519_key) || (pub_key.length() != consts::crypto::x25519_key_length))
        {
            ec = CryptoError::bad_key_type;
            return;
        }

        backend_->dh_x25519(priv_key, pub_key, output, ec);
    }

    void Crypto::hkdf_sha256(
        const seq8_t& salt,
        std::initializer_list<seq32_t> input_key_material,
        SymmetricKey& output1,
        SymmetricKey& output2)
    {
        assert(backend_);
        backend_->hkdf_sha256(salt, input_key_material, output1, output2);
    }

	void Crypto::gen_keypair_Ed25519(KeyPair& pair)
	{
		assert(backend_);
		backend_->gen_keypair_Ed25519(pair);
	}

    void Crypto::inititalize(ICryptoBackend& backend)
    {
        backend_ = &backend;
    }


}


