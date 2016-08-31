
#include "ssp21/crypto/SessionModes.h"

#include "ssp21/crypto/ErrorCodes.h"

namespace ssp21
{

    openpal::RSlice SessionModes::verify_hmac_sha256_trunc16(
        const SymmetricKey& key,
        const openpal::RSlice& msg,
        const openpal::RSlice& payload,
        openpal::WSlice dest,
        std::error_code& ec
    )
    {
        // TODO: actually implement this

        ec = CryptoError::bad_hmac_authentication;
        return openpal::RSlice::empty_slice();
    }


}


