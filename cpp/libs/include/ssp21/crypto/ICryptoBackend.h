
#ifndef SSP21_ICRYTPTOBACKEND_H
#define SSP21_ICRYTPTOBACKEND_H

#include "BufferTypes.h"

#include <initializer_list>
#include <system_error>

namespace ssp21
{
    class ICryptoBackend
    {

    public:

        virtual void zero_memory(const wseq32_t& data) = 0;

        virtual bool secure_equals(const seq8_t& lhs, const seq8_t& rhs) = 0;

        virtual void hash_sha256(std::initializer_list<seq32_t> data, SecureBuffer& output) = 0;

        virtual void hmac_sha256(const seq8_t& key, std::initializer_list<seq32_t> data, SecureBuffer& output) = 0;

        virtual void hkdf_sha256(const seq8_t& chaining_key, std::initializer_list<seq32_t> input_key_material, SymmetricKey& key1, SymmetricKey& key2) = 0;

        virtual void gen_keypair_x25519(KeyPair& pair) = 0;

        virtual void dh_x25519(const PrivateKey& priv_key, const seq8_t& pub_key, DHOutput& output, std::error_code& ec) = 0;
    };
}

#endif
