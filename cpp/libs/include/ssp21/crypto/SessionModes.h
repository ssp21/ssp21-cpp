
#ifndef SSP21_SESSION_MODES_H
#define SSP21_SESSION_MODES_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/gen/SessionMode.h"

#include "ssp21/crypto/BufferTypes.h"

#include <cstdint>
#include <system_error>

namespace ssp21
{
	/**
	* Authenticates a session message payload and places
	* cleartext in the dest output buffer.
	* 
	* @key the symmetric key used for authentication (and optionally decryption)
	* @ad associated data that is also covered by the authentication tag
	* @nonce A nonce that may serve as input into authentication
	* @payload the message payload that contains the data (possibly encrypted) and the authentication tag
	* @dest The output buffer into which the cleartext will be written if no error occurs
	* @ec An error condition will be signaled if the output buffer is too small or if an authentication error occurs
	*
	* @return A slice pointing to the written payload and authentication tag. This slice will be empty if an error occured.
	*/
    typedef openpal::RSlice (*session_read_t)(
        const SymmetricKey& key,
        const openpal::RSlice& ad,
		uint16_t nonce,
        const openpal::RSlice& payload,
        openpal::WSlice dest,
        std::error_code& ec
    );	

    struct SessionModes : private openpal::StaticOnly
    {
        static session_read_t default_session_read()
        {
            return &verify_hmac_sha256_trunc16;
        }

        static openpal::RSlice verify_hmac_sha256_trunc16(
			const SymmetricKey& key,
			const openpal::RSlice& ad,
			uint16_t nonce,
			const openpal::RSlice& payload,
			openpal::WSlice dest,
			std::error_code& ec
        );

    };

}

#endif
