
#ifndef SSP21_CRYTPTO_TYPEDEFS_H
#define SSP21_CRYTPTO_TYPEDEFS_H

#include "ssp21/crypto/BufferTypes.h"

#include <initializer_list>
#include <system_error>

namespace ssp21 {

struct AEADResult {
    std::error_code ec;
    seq32_t ciphertext;
    seq32_t auth_tag;

    static AEADResult success(seq32_t ciphertext, seq32_t auth_tag)
    {
        return AEADResult{ std::error_code(), ciphertext, auth_tag };
    }

    static AEADResult failure(std::error_code ec)
    {
        return AEADResult{ ec, seq32_t::empty(), seq32_t::empty() };
    }
};

using gen_keypair_func_t = void (*)(KeyPair& pair);

using hash_func_t = void (*)(
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output);

using mac_func_t = void (*)(
    const seq32_t& key,
    const std::initializer_list<seq32_t>& data,
    SecureBuffer& output);

using aead_encrypt_t = AEADResult (*)(
    const SymmetricKey& key,
    uint16_t nonce,
    seq32_t ad,
    seq32_t plaintext,
    wseq32_t encrypt_buffer,
    MACOutput& mac);

using aead_decrypt_t = seq32_t (*)(
    const SymmetricKey& key,
    uint16_t nonce,
    seq32_t ad,
    seq32_t ciphertext,
    seq32_t auth_tag,
    wseq32_t plaintext,
    std::error_code& ec);

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
