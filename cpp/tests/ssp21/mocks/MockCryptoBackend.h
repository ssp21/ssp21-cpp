#ifndef SSP21_MOCKCRYPTOBACKEND_H
#define SSP21_MOCKCRYPTOBACKEND_H

#include "ssp21/crypto/ICryptoBackend.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{
    class MockCryptoBackend : public ICryptoBackend, private openpal::Uncopyable
    {

    public:

        struct Counters
        {
            uint32_t num_zero_memory = 0;
            uint32_t num_secure_equals = 0;
            uint32_t num_hash_sha256 = 0;
            uint32_t num_hmac_sha256 = 0;
            uint32_t num_gen_keypair_x25519 = 0;
            uint32_t num_dh_x25519 = 0;

            bool all_zero()
            {
                auto bitwise_or = num_zero_memory |
                                  num_secure_equals |
                                  num_hash_sha256 |
                                  num_hmac_sha256 |
                                  num_gen_keypair_x25519 |
                                  num_dh_x25519;

                return bitwise_or == 0;
            }

            void clear()
            {
                num_zero_memory = 0;
                num_secure_equals = 0;
                num_hash_sha256 = 0;
                num_hmac_sha256 = 0;
                num_gen_keypair_x25519 = 0;
                num_dh_x25519 = 0;
            }
        };

        static MockCryptoBackend instance;

        virtual void zero_memory(openpal::WSlice data) override;

        virtual bool secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs)  override;

        virtual void hash_sha256(std::initializer_list<openpal::RSlice> data, SecureBuffer& output) override;

        virtual void hmac_sha256(const openpal::RSlice& key, std::initializer_list<openpal::RSlice> data, SecureBuffer& output) override;

        virtual void gen_keypair_x25519(KeyPair& pair) override;

        virtual void dh_x25519(const PrivateKey& priv_key, const openpal::RSlice& pub_key, DHOutput& output, std::error_code& ec) override;

        uint8_t fill_byte = 0xFF;

        Counters counters;

    private:

        MockCryptoBackend() {}
    };

}

#endif
