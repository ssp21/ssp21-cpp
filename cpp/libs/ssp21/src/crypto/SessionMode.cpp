
#include "SessionMode.h"

#include "ssp21/crypto/gen/CryptoError.h"

#include <algorithm>

namespace ssp21 {

using metadata_buffer_t = ser4cpp::StaticBuffer<uint32_t, AuthMetadata::fixed_size_bytes>;
using user_data_length_buffer_t = ser4cpp::StaticBuffer<uint32_t, ser4cpp::UInt16::size>;

uint16_t calc_user_data_tx_length(uint32_t total_user_data_len, uint32_t encrypt_buffer_len, uint32_t max_frame_payload)
{
    static constexpr auto MAX_U16_AS_U32 = static_cast<uint32_t>(std::numeric_limits<uint16_t>::max());

    const auto value = std::min(
        std::min(total_user_data_len, encrypt_buffer_len),
        max_frame_payload
    );

    // finally, ensure that the value doesn't exceed U16 MAX before downcasting
    return static_cast<uint16_t>(std::min(value, MAX_U16_AS_U32));
}

seq32_t get_metadata_bytes(const AuthMetadata& metadata, metadata_buffer_t& buffer)
{
    auto dest = buffer.as_wseq();
    metadata.write(dest);
    return buffer.as_seq();
}

seq32_t get_user_data_length_bytes(uint16_t user_data_length, user_data_length_buffer_t& buffer)
{
    auto dest = buffer.as_wseq();
    ser4cpp::UInt16::write_to(dest, user_data_length);
    return buffer.as_seq();
}

SessionMode::SessionMode(aead_encrypt_func_t encrypt, aead_decrypt_func_t decrypt)
    : encrypt(encrypt)
    , decrypt(decrypt)
{
}

seq32_t SessionMode::read(const SymmetricKey& key, const SessionData& msg, wseq32_t dest, std::error_code& ec) const
{
    if (key.get_length_in_bytes() != consts::crypto::symmetric_key_length) {
        ec = CryptoError::bad_buffer_size;
        return seq32_t::empty();
    }

    metadata_buffer_t metadata_buffer;
    const auto ad_bytes = get_metadata_bytes(msg.metadata, metadata_buffer);

    return this->decrypt(key, msg.metadata.nonce, ad_bytes, msg.user_data, msg.auth_tag, dest, ec);
}

SessionData SessionMode::write(const SymmetricKey& key, const AuthMetadata& metadata, seq32_t& user_data, wseq32_t encrypt_buffer, MACOutput& mac, std::error_code& ec) const
{
    if (key.get_length_in_bytes() != consts::crypto::symmetric_key_length) {
        ec = CryptoError::bad_buffer_size;
        return SessionData();
    }

    // we need to be able to encrypt at least one byte
    if (encrypt_buffer.is_empty()) {
        ec = CryptoError::bad_buffer_size;
        return SessionData();
    }

    const uint16_t tx_user_data_length = calc_user_data_tx_length(user_data.length(), encrypt_buffer.length(), 1024); // TODO - make this magic number dependent on the frame type

    metadata_buffer_t buffer;
    const auto ad_bytes = get_metadata_bytes(metadata, buffer);

    const auto result = this->encrypt(key, metadata.nonce, ad_bytes, user_data.take(tx_user_data_length), encrypt_buffer, mac);

    if (result.ec) {
        ec = result.ec;
        return SessionData{};
    }

    const SessionData message(
        metadata,
        result.ciphertext,
        result.auth_tag);

    user_data.advance(tx_user_data_length);

    return message;
}

}