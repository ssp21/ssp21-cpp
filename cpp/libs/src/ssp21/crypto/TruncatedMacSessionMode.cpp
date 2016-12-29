
#include "ssp21/crypto/TruncatedMacSessionMode.h"

#include "ssp21/crypto/gen/CryptoError.h"
#include "ssp21/crypto/Crypto.h"

using namespace openpal;

namespace ssp21
{


    Seq16 TruncatedMacSessionMode::read(
        const SymmetricKey& key,
        const SessionData& msg,
        openpal::WSlice, // ignored in MAC mode
        std::error_code& ec
    ) const
    {
        metadata_buffer_t metadata_buffer;
        const auto ad_bytes = get_metadata_bytes(msg.metadata, metadata_buffer);

        user_data_length_buffer_t length_buffer;
        const auto user_data_length_bytes = get_user_data_length_bytes(msg.user_data.length(), length_buffer);

        // Now calculate the expected MAC
        HashOutput calc_mac_buffer;
        mac_func(key.as_slice().widen<uint32_t>(), { ad_bytes, user_data_length_bytes, msg.user_data.seq.widen<uint32_t>() }, calc_mac_buffer);
        const auto truncated_mac = calc_mac_buffer.as_slice().take(this->auth_tag_length);

        if (!Crypto::secure_equals(msg.auth_tag, truncated_mac)) // authentication failure
        {
            ec = CryptoError::mac_auth_fail;
            return Seq16::empty_slice();
        }

        // we're authenticated, so return the user_data slice
		return msg.user_data;
    }

    Seq16 TruncatedMacSessionMode::write(
        const SymmetricKey& key,
        const AuthMetadata& metadata,
        const Seq16& user_data,
        AuthenticationTag& auth_tag,
        openpal::WSlice,
        std::error_code& ec
    ) const
    {
        // maximum userdata length
        const uint16_t max_userdata_length = UInt16::max_value - this->auth_tag_length;

        // payload must at least have the truncated HMAC
        if (user_data.length() > max_userdata_length)
        {
            ec = CryptoError::bad_buffer_size;
            return Seq16::empty_slice();
        }

        metadata_buffer_t buffer;
        auto ad_bytes = get_metadata_bytes(metadata, buffer);

        user_data_length_buffer_t length_buffer;
        const auto user_data_length_bytes = get_user_data_length_bytes(static_cast<uint16_t>(user_data.length()), length_buffer);

        // Now calculate the mac
        mac_func(key.as_slice().widen<uint32_t>(), { ad_bytes, user_data_length_bytes, user_data.widen<uint32_t>() }, auth_tag);
        auth_tag.set_type(this->buffer_type);

        return user_data;
    }

    uint16_t TruncatedMacSessionMode::max_writable_user_data_length(uint16_t max_payload_size) const
    {
        return (max_payload_size < this->auth_tag_length) ? 0 : max_payload_size - this->auth_tag_length;
    }



}

