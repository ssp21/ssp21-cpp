
#ifndef SSP21_CRYTPTO_TYPEDEFS_H
#define SSP21_CRYTPTO_TYPEDEFS_H

#include "ICryptoBackend.h"

namespace ssp21
{
    typedef void (*gen_keypair_func_t)(KeyPair& pair);

    typedef void (*hash_func_t)(
        std::initializer_list<openpal::RSlice> data,
        SecureBuffer& output
    );

    typedef void (*mac_func_t)(
        const Seq8& key,
        std::initializer_list<openpal::RSlice> data,
        SecureBuffer& output
    );

    typedef void (*dh_func_t)(
        const PrivateKey& priv_key,
        const Seq8& pub_key,
        DHOutput& output,
        std::error_code& ec
    );

    typedef void (*kdf_func_t)(
        const Seq8& chaining_key,
        std::initializer_list<openpal::RSlice> input_key_material,
        SymmetricKey& key1,
        SymmetricKey& key2
    );
}

#endif
