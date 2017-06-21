
#ifndef SSP21_SODIUMBACKEND_H
#define SSP21_SODIUMBACKEND_H

#include "ssp21/crypto/ICryptoBackend.h"

namespace ssp21
{
    /**
    * Crypto backend for libsodium
    */
    class SodiumBackend : public ICryptoBackend
    {

    public:

        SodiumBackend() {}

        static bool initialize();

        virtual void zero_memory(const wseq32_t& data);

        virtual bool secure_equals(const seq32_t& lhs, const seq32_t& rhs);

        virtual void hash_sha256(
            std::initializer_list<seq32_t> data,
            SecureBuffer& output
        );

        virtual void hmac_sha256(
            const seq32_t& key,
            std::initializer_list<seq32_t> data,
            SecureBuffer& output
        );

        virtual void gen_keypair_x25519(KeyPair& pair);

        virtual void dh_x25519(
            const PrivateKey& priv_key,
            const seq32_t& pub_key,
            DHOutput& output,
            std::error_code& ec
        );

        virtual void hkdf_sha256(
            const seq32_t& salt,
            std::initializer_list<seq32_t> input_key_material,
            SymmetricKey& key1,
            SymmetricKey& key2
        );

        virtual void gen_keypair_ed25519(KeyPair& pair);

        virtual void sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec);

        virtual bool verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key);

    };
}

#endif
