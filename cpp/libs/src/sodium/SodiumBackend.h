

#ifndef SODIUMBACKEND_BACKEND_H
#define SODIUMBACKEND_BACKEND_H

#include "ssp21/crypto/ICryptoBackend.h"

namespace ssp21
{
    namespace sodium
    {
        class SodiumBackend final : public ICryptoBackend
        {

        public:

            virtual void zero_memory(wseq32_t data) override;

            virtual bool secure_equals(const seq8_t& lhs, const seq8_t& rhs) override;

            virtual void hash_sha256(std::initializer_list<seq32_t> data, SecureBuffer& output) override;

            virtual void hmac_sha256(const seq8_t& key, std::initializer_list<seq32_t> data,
                                     SecureBuffer& output) override;

            virtual void gen_keypair_x25519(KeyPair& pair) override;

            virtual void dh_x25519(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec) override;

            static ICryptoBackend& Instance()
            {
                return backend_;
            }

        private:

            static SodiumBackend backend_;
        };

    }
}

#endif
