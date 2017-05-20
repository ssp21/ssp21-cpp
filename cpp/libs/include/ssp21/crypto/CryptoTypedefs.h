
#ifndef SSP21_CRYTPTO_TYPEDEFS_H
#define SSP21_CRYTPTO_TYPEDEFS_H

#include "ssp21/crypto/BufferTypes.h"

#include <system_error>

namespace ssp21
{
    typedef void (*gen_keypair_func_t)(KeyPair& pair);

    typedef void (*hash_func_t)(
        std::initializer_list<seq32_t> data,
        SecureBuffer& output
    );

    typedef void (*mac_func_t)(
        const seq8_t& key,
        std::initializer_list<seq32_t> data,
        SecureBuffer& output
    );

    typedef void (*dh_func_t)(
        const PrivateKey& priv_key,
        const seq8_t& pub_key,
        DHOutput& output,
        std::error_code& ec
    );

    typedef void (*kdf_func_t)(
        const seq8_t& chaining_key,
        std::initializer_list<seq32_t> input_key_material,
        SymmetricKey& key1,
        SymmetricKey& key2
    );

    typedef bool(*verify_dsa_t)(
        const seq32_t& message,
        const seq32_t& signature,
        const seq32_t& public_key
    );
}

#endif
