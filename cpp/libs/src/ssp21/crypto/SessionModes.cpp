
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
		const openpal::RSlice& ad,
		uint16_t nonce,
		const openpal::RSlice& payload,
		openpal::WSlice& dest,
		std::error_code& ec
    )
    {        
		// AD length must be a u16
		if (ad.length() > UInt16::max_value) {
			ec = CryptoError::bad_buffer_size;
			return RSlice::empty_slice();
		}

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

		const uint16_t ad_length_u16 = static_cast<uint16_t>(ad.length());
				
		// split the payload into user data and MAC
		const auto user_data = payload.take(user_data_length);
		const auto read_mac = payload.skip(user_data_length);
		
		openpal::StaticBuffer<4> nonce_and_ad_length;
		
		{
			auto dest = nonce_and_ad_length.as_wslice();
			if (!BigEndian::write(dest, nonce, ad_length_u16) || dest.is_not_empty())
			{
				ec = CryptoError::bad_buffer_size;
				return RSlice::empty_slice();
			}
		}

		// Now calculate the expected MAC
		HashOutput calc_mac_buffer;
		mac_func(key.as_slice(), { nonce_and_ad_length.as_rslice(), ad, user_data }, calc_mac_buffer);
		const auto truncated = calc_mac_buffer.as_slice().take(trunc_length);
		
		if (!Crypto::secure_equals(read_mac, truncated)) // authentication failure
		{
			ec = CryptoError::mac_auth_fail;
			return openpal::RSlice::empty_slice();
		}		

		// we're authenticated, transfer the user data and return the slice
		return user_data.copy_to(dest);        
    }


}


