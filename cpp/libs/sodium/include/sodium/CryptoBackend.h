
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

        virtual void zero_memory(const wseq32_t& data) override;

        virtual void gen_random(const wseq32_t& dest) override;

        virtual bool secure_equals(const seq32_t& lhs, const seq32_t& rhs) override;

        virtual void hash_sha256(
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output) override;

        virtual void hmac_sha256(
            const seq32_t& key,
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output) override;

        virtual void gen_keypair_x25519(KeyPair& pair) override;

        virtual void dh_x25519(
            const PrivateKey& priv_key,
            const seq32_t& pub_key,
            DHOutput& output,
            std::error_code& ec) override;

        virtual void hkdf_sha256(
            const seq32_t& salt,
            const std::initializer_list<seq32_t>& input_key_material,
            SymmetricKey& key1,
            SymmetricKey& key2) override;

        virtual void gen_keypair_ed25519(KeyPair& pair) override;

        virtual void sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec) override;

        virtual bool verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key) override;
    };

}
}

#endif
