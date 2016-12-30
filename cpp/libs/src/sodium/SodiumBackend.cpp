

#include "SodiumBackend.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <sodium.h>
#include <assert.h>

namespace ssp21
{
    namespace sodium
    {
        /// assertions for SHA-256 related constants
        static_assert(crypto_hash_sha256_BYTES == crypto_auth_hmacsha256_BYTES, "sha256 hash and HMAC length mismatch");
        static_assert(consts::crypto::sha256_hash_output_length == crypto_hash_sha256_BYTES, "sha256 length mismatch");
        static_assert(consts::crypto::sha256_hash_output_length == crypto_auth_hmacsha256_BYTES, "sha256-HMAC length mismatch");

        /// assertions for DH key lengths
        static_assert(consts::crypto::x25519_key_length == crypto_scalarmult_BYTES, "X25519 key length mismatch");

        SodiumBackend SodiumBackend::backend_;

        void SodiumBackend::zero_memory(wseq32_t data)
        {
            sodium_memzero(data, data.length());
        }

        bool SodiumBackend::secure_equals(const seq8_t& lhs, const seq8_t& rhs)
        {
            if (lhs.length() != rhs.length()) return false;

            return sodium_memcmp(lhs, rhs, lhs.length()) == 0;
        }

        void SodiumBackend::hash_sha256(std::initializer_list<seq32_t> data, SecureBuffer& output)
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

        void SodiumBackend::hmac_sha256(const seq8_t& key, std::initializer_list<seq32_t> data, SecureBuffer& output)
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

        void SodiumBackend::gen_keypair_x25519(KeyPair& pair)
        {
            auto dest = pair.private_key.as_wseq();
            randombytes_buf(dest, crypto_scalarmult_BYTES);
            crypto_scalarmult_base(pair.public_key.as_wseq(), dest);

            pair.public_key.set_type(BufferType::x25519_key);
            pair.private_key.set_type(BufferType::x25519_key);
        }

        void SodiumBackend::dh_x25519(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec)
        {
            if (crypto_scalarmult(output.as_wseq(), priv_key.as_seq(), pub_key) != 0)
            {
                ec = ssp21::CryptoError::dh_x25519_fail;
                return;
            }

            output.set_type(BufferType::x25519_key);
        }
    }
}
