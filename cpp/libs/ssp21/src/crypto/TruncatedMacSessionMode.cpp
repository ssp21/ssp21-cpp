
#include "crypto/TruncatedMacSessionMode.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include <algorithm>

namespace ssp21 {
seq32_t TruncatedMacSessionMode::read_impl(
    const SymmetricKey& key,
    const SessionData& msg,
    wseq32_t dest,
    std::error_code& ec) const
{
    metadata_buffer_t metadata_buffer;
    const auto ad_bytes = get_metadata_bytes(msg.metadata, metadata_buffer);

    user_data_length_buffer_t length_buffer;
    const auto user_data_length_bytes = get_user_data_length_bytes(msg.user_data.length(), length_buffer);

    // Now calculate the expected MAC
    HashOutput calc_mac_buffer;
    mac_func(key.as_seq(), { ad_bytes, user_data_length_bytes, msg.user_data }, calc_mac_buffer);
    const auto truncated_mac = calc_mac_buffer.as_seq().take(this->auth_tag_length);

    if (!Crypto::secure_equals(msg.auth_tag, truncated_mac)) // authentication failure
    {
        ec = CryptoError::mac_auth_fail;
        return seq32_t::empty();
    }

    // we're authenticated, copy the user data to the destination buffer
    if (dest.length() < msg.user_data.length()) {
        ec = CryptoError::bad_buffer_size;
        return seq32_t::empty();
    }

    return dest.copy_from(msg.user_data);
}

SessionData TruncatedMacSessionMode::write_impl(
    const SymmetricKey& key,
    const AuthMetadata& metadata,
    seq32_t& user_data,
    wseq32_t encrypt_buffer,
    MACOutput& mac,
    std::error_code& ec) const
{
    // we need to be able to encrypt at least one byte
    if (encrypt_buffer.is_empty()) {
        ec = CryptoError::bad_buffer_size;
        return SessionData();
    }

    // we transmit the smaller of the encrypt buffer size, the user_data size, or u16 max value
    const uint16_t tx_user_data_length = static_cast<uint16_t>(
        std::min(
            std::min(user_data.length(), encrypt_buffer.length()),
            static_cast<uint32_t>(std::numeric_limits<uint16_t>::max())));

    metadata_buffer_t buffer;
    auto ad_bytes = get_metadata_bytes(metadata, buffer);

    user_data_length_buffer_t length_buffer;
    const auto user_data_length_bytes = get_user_data_length_bytes(tx_user_data_length, length_buffer);

    // Now calculate the mac
    mac_func(key.as_seq(), { ad_bytes, user_data_length_bytes, user_data }, mac);

    const SessionData message(
        metadata,
        user_data.take(tx_user_data_length),
        mac.as_seq().take(this->auth_tag_length) // truncate the MAC
    );

    user_data.advance(tx_user_data_length);

    return message;
}

}
