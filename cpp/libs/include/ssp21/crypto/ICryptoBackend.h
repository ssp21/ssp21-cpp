
#ifndef SSP21_ICRYTPTOBACKEND_H
#define SSP21_ICRYTPTOBACKEND_H

#include "ssp21/crypto/BufferTypes.h"

#include <openpal/util/Uncopyable.h>

#include <system_error>
#include <initializer_list>

namespace ssp21
{
    /**
    * Pluggable crypto backend
    */
    class ICryptoBackend : private openpal::Uncopyable
    {

    public:

        virtual ~ICryptoBackend() {}

        virtual void zero_memory(const wseq32_t& data) = 0;

        virtual bool secure_equals(const seq32_t& lhs, const seq32_t& rhs) = 0;

        virtual void hash_sha256(
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output
        ) = 0;

        virtual void hmac_sha256(
            const seq32_t& key,
            const std::initializer_list<seq32_t>& data,
            SecureBuffer& output
        ) = 0;

        virtual void gen_keypair_x25519(KeyPair& pair) = 0;

        virtual void dh_x25519(
            const PrivateKey& priv_key,
            const seq32_t& pub_key,
            DHOutput& output,
            std::error_code& ec
        ) = 0;

        virtual void hkdf_sha256(
            const seq32_t& salt,
            const std::initializer_list<seq32_t>& input_key_material,
            SymmetricKey& key1,
            SymmetricKey& key2
        ) = 0;

        virtual void gen_keypair_ed25519(KeyPair& pair) = 0;

        virtual void sign_ed25519(const seq32_t& input, const seq32_t& private_key, DSAOutput& output, std::error_code& ec) = 0;

        virtual bool verify_ed25519(const seq32_t& message, const seq32_t& signature, const seq32_t& public_key) = 0;

    };
}

#endif
