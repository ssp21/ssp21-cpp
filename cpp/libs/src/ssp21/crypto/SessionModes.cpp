
#include "ssp21/crypto/SessionModes.h"

#include "ssp21/crypto/CryptoErrorCategory.h"
#include "openpal/serialization/BigEndian.h"

using namespace openpal;

namespace ssp21
{

    openpal::RSlice SessionModes::read_any_mac_with_truncation(
        mac_func_t mac_func,
        uint8_t trunc_length,
        const SymmetricKey& key,
        const SessionAuthData& ad,
        const openpal::RSlice& payload,
        openpal::WSlice& dest,
        std::error_code& ec)
    {
        // payload must at least have the truncated HMAC
        if (payload.length() < trunc_length)
        {
            ec = CryptoError::bad_buffer_size;
            return RSlice::empty_slice();
        }

        const auto user_data_length = payload.length() - trunc_length;

        if (dest.length() < user_data_length) // insufficient space to write cleartext
        {
            ec = CryptoError::bad_buffer_size;
            return RSlice::empty_slice();
        }

        ad_buffer_t buffer;
        auto ad_bytes = get_ad_bytes(ad, buffer);

        // split the payload into user data and MAC
        const auto user_data = payload.take(user_data_length);
        const auto read_mac = payload.skip(user_data_length);

        // Now calculate the expected MAC
        HashOutput calc_mac_buffer;
        mac_func(key.as_slice(), { ad_bytes, user_data }, calc_mac_buffer);
        const auto truncated = calc_mac_buffer.as_slice().take(trunc_length);

        if (!Crypto::secure_equals(read_mac, truncated)) // authentication failure
        {
            ec = CryptoError::mac_auth_fail;
            return openpal::RSlice::empty_slice();
        }

        // we're authenticated, transfer the user data and return the slice
        return user_data.copy_to(dest);
    }

    openpal::RSlice SessionModes::write_any_mac_with_truncation(
        mac_func_t mac_func,
        uint8_t trunc_length,
        const SymmetricKey& key,
        const SessionAuthData& ad,
        const openpal::RSlice& userdata,
        openpal::WSlice& dest,
        std::error_code& ec)
    {
        // maximum userdata length
        const uint16_t max_userdata_length = UInt16::max_value - trunc_length;

        // payload must at least have the truncated HMAC
        if (userdata.length() > max_userdata_length)
        {
            ec = CryptoError::bad_buffer_size;
            return RSlice::empty_slice();
        }

        const uint16_t payload_length = userdata.length() + trunc_length;

        if(dest.length() < payload_length)
        {
            ec = CryptoError::bad_buffer_size;
            return RSlice::empty_slice();
        }

        ad_buffer_t buffer;
        auto ad_bytes = get_ad_bytes(ad, buffer);

        // Now calculate the mac
        HashOutput calc_mac_buffer;
        mac_func(key.as_slice(), { ad_bytes, userdata }, calc_mac_buffer);
        const auto truncated = calc_mac_buffer.as_slice().take(trunc_length);

        const auto ret = dest.as_rslice().take(payload_length);

        // write the output buffer, advancing dest
        userdata.copy_to(dest);
        truncated.copy_to(dest);

        return ret;
    }

    openpal::RSlice SessionModes::get_ad_bytes(const SessionAuthData& ad, ad_buffer_t& buffer)
    {
        auto dest = buffer.as_wslice();
        ad.write(dest);
        return buffer.as_rslice();
    }
}


