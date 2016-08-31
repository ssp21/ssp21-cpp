
#include "ssp21/crypto/SessionModes.h"

#include "ssp21/crypto/CryptoErrorCategory.h"

#include <openpal/serialization/BigEndian.h>

using namespace openpal;

namespace ssp21
{

    openpal::RSlice SessionModes::verify_hmac_sha256_trunc16(
		const SymmetricKey& key,
		const openpal::RSlice& ad,
		uint16_t nonce,
		const openpal::RSlice& payload,
		openpal::WSlice dest,
		std::error_code& ec
    )
    {        
		// preconditions
		if (ad.length() > UInt16::size) {
			//ec = 
			return RSlice::empty_slice();
		}


		openpal::StaticBuffer<2> nonce_bytes;
		openpal::StaticBuffer<2> ad_length_bytes;



        ec = CryptoError::bad_hmac_authentication;
        return openpal::RSlice::empty_slice();
    }


}


