
#ifndef SSP21_CRYTPTO_TYPEDEFS_H
#define SSP21_CRYTPTO_TYPEDEFS_H

#include "ssp21/crypto/BufferTypes.h"

#include <initializer_list>
#include <system_error>

namespace ssp21 {
using gen_keypair_func_t = void (*)(KeyPair& pair);

using hash_func_t = void (*)(
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output);

using mac_func_t = void (*)(
    const seq32_t& key,
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output);

using dh_func_t = void (*)(
    const PrivateKey& priv_key,
    const seq32_t& pub_key,
    DHOutput& output,
    std::error_code& ec);

using kdf_func_t = void (*)(
    const seq32_t& salt,
    const std::initializer_list<seq32_t>& input_key_material,
    SymmetricKey& key1,
    SymmetricKey& key2);

using verify_dsa_t = bool (*)(
    const seq32_t& message,
    const seq32_t& signature,
    const seq32_t& public_key);
}

#endif
