
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

        virtual void zero_memory(openpal::WSlice data) = 0;

        virtual bool secure_equals(const Seq8& lhs, const Seq8& rhs) = 0;

        virtual void hash_sha256(std::initializer_list<Seq32> data, SecureBuffer& output) = 0;

        virtual void hmac_sha256(const Seq8& key, std::initializer_list<Seq32> data, SecureBuffer& output) = 0;

        virtual void gen_keypair_x25519(KeyPair& pair) = 0;

        virtual void dh_x25519(const PrivateKey& priv_key, const Seq32& pub_key, DHOutput& output, std::error_code& ec) = 0;
    };
}

#endif
