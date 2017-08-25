
#include "ssp21/crypto/TruncatedMacSessionMode.h"

#include "ssp21/crypto/gen/CryptoError.h"
#include "ssp21/crypto/Crypto.h"

using namespace openpal;

namespace ssp21
{
    seq32_t TruncatedMacSessionMode::read_impl(
        const SymmetricKey& key,
        const SessionData& msg,
        wseq32_t, // ignored in MAC mode
        std::error_code& ec
    ) const
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

        // we're authenticated, so return the user_data slice
        return msg.user_data;
    }

    seq32_t TruncatedMacSessionMode::write_impl(
        IFrameWriter& writer,
        const SymmetricKey& key,
        AuthMetadata& metadata,
        seq32_t& user_data,
        const wseq32_t& encrypt_scratch_space,
        std::error_code& ec
    ) const
    {
        // first calculate how much user data we can place in the message
        const uint16_t max_message_size = writer.get_max_payload_size();

        const uint16_t min_message_size = SessionData::min_size_bytes + this->auth_tag_length;

        if (max_message_size <= min_message_size) // we have to be able to write at least one byte of payload
        {
            ec = CryptoError::bad_buffer_size;
            return seq32_t::empty();
        }

        // the maximum amount of user data we could conceivably transmit
        const uint16_t max_tx_user_data_length = max_message_size - min_message_size;        

        // the actual amount we're going to try to transmit
        const uint16_t tx_user_data_length = user_data.length() < max_tx_user_data_length ? static_cast<uint16_t>(user_data.length()) : max_tx_user_data_length;

        metadata_buffer_t buffer;
        auto ad_bytes = get_metadata_bytes(metadata, buffer);

        user_data_length_buffer_t length_buffer;
        const auto user_data_length_bytes = get_user_data_length_bytes(tx_user_data_length, length_buffer);

        HashOutput tag;

        // Now calculate the mac
        mac_func(key.as_seq(), { ad_bytes, user_data_length_bytes, user_data }, tag);

        const auto trunacted_mac = tag.as_seq().take(this->auth_tag_length);

        SessionData message(
            metadata,
            user_data.take(tx_user_data_length),
            trunacted_mac // truncate the MAC
        );

        const auto res = writer.write(message);

        if (res.is_error())
        {
            ec = res.err;
            return seq32_t::empty();
        }

        user_data.advance(tx_user_data_length);

        return res.frame;
    }

}

