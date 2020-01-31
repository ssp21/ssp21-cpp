
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include "ssp21/crypto/CryptoBackend.h"

#include <memory>

namespace ssp21 {

/**
* Assumming for the time being that a static backend is fine
* to keep dependency injection simple.
*/
class Crypto final : ser4cpp::StaticOnly {

    // flag to say if we've intialized the backend
    static bool initialized;

    // the actual backend
    static CryptoBackend backend;

    static void check_initialized();
    static void check_supports_sha256();
    static void check_supports_x25519();
    static void check_supports_ed25519();
    static void check_supports_aes256_gcm();

public:
    /**
	 * Intialize a backend. Callable only once.
	 *
	 * Throws an exception if the backend doesn't support mandatory primitives.	 
	 */
    static void initialize(const CryptoBackend& backend);

    // --- required functions can be called directly ---

    static void zero_memory(const wseq32_t& data);

    static void gen_random(const wseq32_t& dest);

    static bool secure_equals(const seq32_t& lhs, const seq32_t& rhs);

    // --- optional primitives must be checked for support ---

    // supports SHA-256 && HMAC-SHA-256 && HKDF-SHA-256
    static bool supports_sha256();
    // supports x25519 keygen and DH
    static bool supports_x25519();
    // supports ed25519 keygen, sign, and verify
    static bool supports_ed25519();
    // supports AES-GCM encrypt/decrypt
    static bool supports_aes256_gcm();

    // --- optional primitives will exit application if called with no support ---

    static void hash_sha256(
        const std::initializer_list<seq32_t>& data,
        SecureBuffer& output);

    static void hmac_sha256(
        const seq32_t& key,
        const std::initializer_list<seq32_t>& data,
        SecureBuffer& output);

    static void gen_keypair_x25519(KeyPair& pair);

    static void dh_x25519(
        const PrivateKey& priv_key,
        const seq32_t& pub_key,
        DHOutput& output,
        std::error_code& ec);

    static void hkdf_sha256(
        const seq32_t& salt,
        const std::initializer_list<seq32_t>& input_key_material,
        SymmetricKey& key1,
        SymmetricKey& key2);

    static void gen_keypair_ed25519(KeyPair& pair);

    static void sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec);

    static bool verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key);

    static AEADResult aes256_gcm_encrypt(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t plaintext, wseq32_t encrypt_buffer, MACOutput& mac);

    static seq32_t aes256_gcm_decrypt(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t ciphertext, seq32_t auth_tag, wseq32_t plaintext, std::error_code& ec);
};
}

#endif
