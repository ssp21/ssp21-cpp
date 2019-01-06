
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include "ssp21/crypto/ICryptoBackend.h"

#include <memory>

namespace ssp21
{
    /**
    * Assumming for the time being that a static backend is fine
    * to keep dependency injection simple.
    */
    class Crypto final : openpal::StaticOnly
    {

        static std::shared_ptr<ICryptoBackend> backend;

    public:

        // --- Inject a backend. Callable only once ---

        static bool initialize(const std::shared_ptr<ICryptoBackend>& backend);

        // --- proxy functions that invoke the static backend, enforcing preconditions ---

        static void zero_memory(const wseq32_t& data);

        static void gen_random(const wseq32_t& dest);

        static bool secure_equals(const seq32_t& lhs, const seq32_t& rhs);

        static void hash_sha256(
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output
        );

        static void hmac_sha256(
            const seq32_t& key,
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output
        );

        static void gen_keypair_x25519(KeyPair& pair);

        static void dh_x25519(
            const PrivateKey& priv_key,
            const seq32_t& pub_key,
            DHOutput& output,
            std::error_code& ec
        );

        static void hkdf_sha256(
            const seq32_t& salt,
            const std::initializer_list<seq32_t>& input_key_material,
            SymmetricKey& key1,
            SymmetricKey& key2
        );

        static void gen_keypair_ed25519(KeyPair& pair);

        static void sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec);

        static bool verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key);

    };
}

#endif
