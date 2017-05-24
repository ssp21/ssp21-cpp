
#include "ssp21/crypto/Crypto.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <assert.h>

namespace ssp21
{
    bool Crypto::initialized(false);

    bool Crypto::initialize()
    {
        assert(!initialized);
        initialized = initialize_impl();
        return initialized;
    }

    void Crypto::zero_memory(const wseq32_t& data)
    {
        assert(initialized);
        zero_memory_impl(data);
    }

    bool Crypto::secure_equals(const seq32_t& lhs, const seq32_t& rhs)
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
        const seq32_t& key,
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

    void Crypto::dh_x25519(const PrivateKey& priv_key, const seq32_t& pub_key, DHOutput& output, std::error_code& ec)
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
        const seq32_t& salt,
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

    void Crypto::sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec)
    {
        assert(initialized);
        if (private_key.length() != consts::crypto::ed25519_private_key_length)
        {
            ec = CryptoError::bad_length;
            return;
        }

        sign_ed25519_impl(input, private_key, output, ec);
    }

    bool Crypto::verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key)
    {
        assert(initialized);
        if (public_key.length() != consts::crypto::ed25519_public_key_length)
        {
            return false;
        }

        if (signature.length() != consts::crypto::ed25519_signature_length)
        {
            return false;
        }

        return verify_ed25519_impl(message, signature, public_key);
    }


}


