
#ifndef SSP21_SODIUM_CRYPTOBACKEND_H
#define SSP21_SODIUM_CRYPTOBACKEND_H

#include "ssp21/crypto/ICryptoBackend.h"

namespace ssp21 {
namespace sodium {

    /**
		* Crypto backend for libsodium
		*/
    class CryptoBackend : public ICryptoBackend {

    public:
        static bool initialize();

        void zero_memory(const wseq32_t& data) override;

        void gen_random(const wseq32_t& dest) override;

        virtual bool secure_equals(const seq32_t& lhs, const seq32_t& rhs) override;

        void hash_sha256(
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output) override;

        void hmac_sha256(
            const seq32_t& key,
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output) override;

        void gen_keypair_x25519(KeyPair& pair) override;

        void dh_x25519(
            const PrivateKey& priv_key,
            const seq32_t& pub_key,
            DHOutput& output,
            std::error_code& ec) override;

        void hkdf_sha256(
            const seq32_t& salt,
            const std::initializer_list<seq32_t>& input_key_material,
            SymmetricKey& key1,
            SymmetricKey& key2) override;

        void gen_keypair_ed25519(KeyPair& pair) override;

        void sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec) override;

        bool verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key) override;

		AEADResult aes256_gcm_encrypt(const SymmetricKey& key, uint16_t nonce, seq32_t ad, seq32_t plaintext, wseq32_t encrypt_buffer, MACOutput& mac) override;
    };

}
}

#endif
