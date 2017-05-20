

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include "HKDF.h"

#include <sodium.h>
#include <assert.h>

namespace ssp21
{

    // assertions for SHA-256 related constants
    static_assert(crypto_hash_sha256_BYTES == crypto_auth_hmacsha256_BYTES, "sha256 hash and HMAC length mismatch");
    static_assert(consts::crypto::sha256_hash_output_length == crypto_hash_sha256_BYTES, "sha256 length mismatch");
    static_assert(consts::crypto::sha256_hash_output_length == crypto_auth_hmacsha256_BYTES, "sha256-HMAC length mismatch");

    // assertions for DH key lengths
    static_assert(consts::crypto::x25519_key_length == crypto_scalarmult_BYTES, "X25519 key length mismatch");

    // assertions for DSA key/signature lengths
    static_assert(consts::crypto::ed25519_public_key_length == crypto_sign_PUBLICKEYBYTES, "ed25519 public key length mismatch");
    static_assert(consts::crypto::ed25519_private_key_length == crypto_sign_SECRETKEYBYTES, "ed25519 private key length mismatch");
    static_assert(consts::crypto::ed25519_signature_length == crypto_sign_BYTES, "ed25519 signature length mismatch");

    bool Crypto::initialize_impl()
    {
        return sodium_init() == 0;
    }

    void Crypto::zero_memory_impl(const wseq32_t& data)
    {
        sodium_memzero(data, data.length());
    }

    bool Crypto::secure_equals_impl(const seq8_t& lhs, const seq8_t& rhs)
    {
        if (lhs.length() != rhs.length()) return false;

        return sodium_memcmp(lhs, rhs, lhs.length()) == 0;
    }

    void Crypto::hash_sha256_impl(std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
        crypto_hash_sha256_state state;
        crypto_hash_sha256_init(&state);

        for (auto& item : data)
        {
            crypto_hash_sha256_update(&state, item, item.length());
        }

        crypto_hash_sha256_final(&state, output.as_wseq());

        output.set_type(BufferType::sha256);
    }

    void Crypto::hmac_sha256_impl(const seq8_t& key, std::initializer_list<seq32_t> data, SecureBuffer& output)
    {
        crypto_auth_hmacsha256_state state;
        crypto_auth_hmacsha256_init(&state, key, key.length());

        for (auto& item : data)
        {
            crypto_auth_hmacsha256_update(&state, item, item.length());
        }


        crypto_auth_hmacsha256_final(&state, output.as_wseq());

        output.set_type(BufferType::sha256);
    }

    void Crypto::hkdf_sha256_impl(const seq8_t& salt, std::initializer_list<seq32_t> input_key_material, SymmetricKey& key1, SymmetricKey& key2)
    {
        ssp21::hkdf<hmac_sha256_impl>(salt, input_key_material, key1, key2);
    }

    void Crypto::gen_keypair_x25519_impl(KeyPair& pair)
    {
        auto dest = pair.private_key.as_wseq();
        randombytes_buf(dest, crypto_scalarmult_BYTES);

        // TODO - libsodium, undocumented error code?
        crypto_scalarmult_base(pair.public_key.as_wseq(), dest);

        pair.public_key.set_type(BufferType::x25519_key);
        pair.private_key.set_type(BufferType::x25519_key);
    }

    void Crypto::dh_x25519_impl(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec)
    {
        if (crypto_scalarmult(output.as_wseq(), priv_key.as_seq(), pub_key) != 0)
        {
            ec = ssp21::CryptoError::dh_x25519_fail;
            return;
        }

        output.set_type(BufferType::x25519_key);
    }

    void Crypto::gen_keypair_ed25519_impl(KeyPair& pair)
    {
        auto publicDest = pair.public_key.as_wseq();
        auto privateDest = pair.private_key.as_wseq();

        // TODO - libsodium, undocumented error code?
        crypto_sign_keypair(publicDest, privateDest);

        pair.public_key.set_type(BufferType::ed25519_public_key);
        pair.private_key.set_type(BufferType::ed25519_private_key);
    }

    void Crypto::sign_ed25519_impl(const seq32_t& input, const PrivateKey& key, DSAOutput& output, std::error_code& ec)
    {
        // TODO - libsodium, undocumented error code?
        crypto_sign_detached(output.as_wseq(), nullptr, input, input.length(), key.as_seq());

        output.set_type(BufferType::ed25519_signature);
    }

    void Crypto::verify_ed25519_impl(const seq32_t& message, const seq32_t& signature, const PublicKey& key, std::error_code& ec)
    {
        const auto result = crypto_sign_verify_detached(signature, message, message.length(), key.as_seq());
        if (result != 0)
        {
            ec = CryptoError::signature_auth_fail;
        }
    }

}
