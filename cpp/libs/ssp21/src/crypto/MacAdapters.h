
#ifndef SSP21_MAC_ADAPTERS_H
#define SSP21_MAC_ADAPTERS_H

#include "ssp21/crypto/CryptoTypedefs.h"

#include <ser4cpp/container/StaticBuffer.h>
#include <ser4cpp/serialization/BigEndian.h>

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/gen/CryptoError.h"

namespace ssp21 {

// converts a mac_func_t signature into an aead_encrypt_func_t signature
// in accordance with the SSP21 spec
template <class MAC, uint8_t TRUNC>
AEADResult aead_mac_encrypt(
    const SymmetricKey& key,
    uint16_t nonce,
    seq32_t ad,
    seq32_t cleartext,
    wseq32_t encrypt_buffer,
    MACOutput& mac)
{
    if (cleartext.length() > std::numeric_limits<uint16_t>::max()) {
        return AEADResult::failure(CryptoError::bad_length);
    }

    const auto length = static_cast<uint16_t>(cleartext.length());

    ser4cpp::StaticBuffer<uint32_t, ser4cpp::UInt16::size> length_buffer;
    {
        auto dest = length_buffer.as_wseq();
        ser4cpp::UInt16::write_to(dest, length);
    }

    // set auth_tag = MAC(key, ad || len(cleartext) || cleartext)
    MAC::calc(key.as_seq(), { ad, length_buffer.as_seq(), cleartext }, mac);

    return AEADResult::success(
        cleartext, // ciphertext is just the cleartext for auth-only MAC modes
        mac.as_seq().take(TRUNC));
}

// converts a mac_func_t signature into an aead_decrypt_func_t signature
// in accordance with the SSP21 spec
template <class MAC, uint8_t TRUNC>
seq32_t aead_mac_decrypt(
    const SymmetricKey& key,
    uint16_t nonce,
    seq32_t ad,
    seq32_t ciphertext,
    seq32_t auth_tag,
    wseq32_t cleartext,
    std::error_code& ec)
{
    if (cleartext.length() > std::numeric_limits<uint16_t>::max()) {
        ec = CryptoError::bad_length;
        return seq32_t::empty();
    }

    const auto length = static_cast<uint16_t>(ciphertext.length());

    ser4cpp::StaticBuffer<uint32_t, ser4cpp::UInt16::size> length_buffer;
    {
        auto dest = length_buffer.as_wseq();
        ser4cpp::UInt16::write_to(dest, length);
    }

    MACOutput mac;

    // set auth_tag = MAC(key, ad || len(cleartext) || cleartext)
    MAC::calc(key.as_seq(), { ad, length_buffer.as_seq(), ciphertext }, mac);

    // verify the MAC
    if (!Crypto::secure_equals(mac.as_seq().take(TRUNC), auth_tag)) {
        ec = CryptoError::mac_auth_fail;
        return seq32_t::empty();
    }

    // the ciphertext is plaintext in auth-only modes
    return ciphertext;
}

}

#endif
