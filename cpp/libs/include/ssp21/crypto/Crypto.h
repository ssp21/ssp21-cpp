
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include "ssp21/crypto/BufferTypes.h"

#include <system_error>

#include <openpal/util/Uncopyable.h>

namespace ssp21
{
    /**
    * Assumming for the time being that a static backend is fine
    * to keep dependency injection simple.
    */
    class Crypto final : openpal::StaticOnly
    {

        static bool initialized;

    public:

        // --- These are the static proxy functions that SSP21 calls at runtime ---

        static bool initialize();

        static void zero_memory(const wseq32_t& data);

        static bool secure_equals(const seq8_t& lhs, const seq8_t& rhs);

        static void hash_sha256(
            std::initializer_list<seq32_t> data,
            SecureBuffer& output
        );

        static void hmac_sha256(
            const seq8_t& key,
            std::initializer_list<seq32_t> data,
            SecureBuffer& output
        );

        static void gen_keypair_x25519(KeyPair& pair);

        static void dh_x25519(
            const PrivateKey& priv_key,
            const seq8_t& pub_key,
            DHOutput& output,
            std::error_code& ec
        );

        static void hkdf_sha256(
            const seq8_t& salt,
            std::initializer_list<seq32_t> input_key_material,
            SymmetricKey& key1,
            SymmetricKey& key2
        );

        static void gen_keypair_ed25519(KeyPair& pair);

        static void sign_ed25519(const seq32_t& input, const PrivateKey& key, DSAOutput& output, std::error_code& ec);

        static void verify_ed25519(const seq32_t& message, const seq32_t& signature, const PublicKey& key, std::error_code& ec);

    private:

        // The implementation is linked into the final program

        static bool initialize_impl();

        static void zero_memory_impl(const wseq32_t& data);

        static bool secure_equals_impl(const seq8_t& lhs, const seq8_t& rhs);

        static void hash_sha256_impl(
            std::initializer_list<seq32_t> data,
            SecureBuffer& output
        );

        static void hmac_sha256_impl(
            const seq8_t& key,
            std::initializer_list<seq32_t> data,
            SecureBuffer& output
        );

        static void gen_keypair_x25519_impl(KeyPair& pair);

        static void dh_x25519_impl(
            const PrivateKey& priv_key,
            const seq8_t& pub_key,
            DHOutput& output,
            std::error_code& ec
        );

        static void hkdf_sha256_impl(
            const seq8_t& salt,
            std::initializer_list<seq32_t> input_key_material,
            SymmetricKey& key1,
            SymmetricKey& key2
        );

        static void gen_keypair_ed25519_impl(KeyPair& pair);

        static void sign_ed25519_impl(const seq32_t& input, const PrivateKey& key, DSAOutput& output, std::error_code& ec);

        static void verify_ed25519_impl(const seq32_t& message, const seq32_t& signature, const PublicKey& key, std::error_code& ec);

    };
}

#endif
