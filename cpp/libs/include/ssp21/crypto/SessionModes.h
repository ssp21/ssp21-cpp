
#ifndef SSP21_SESSION_MODES_H
#define SSP21_SESSION_MODES_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/gen/SessionMode.h"

#include "ssp21/crypto/BufferTypes.h"

#include <cstdint>
#include <system_error>

namespace ssp21
{
	typedef openpal::RSlice (*session_verify_t)(
		const SymmetricKey& key,
		const openpal::RSlice& msg,
		const openpal::RSlice& payload,
		openpal::WSlice dest,
		std::error_code& ec
	);

	struct SessionModes : private openpal::StaticOnly
	{		
		static session_verify_t default_session_mode() { return &verify_hmac_sha256_trunc16; }

		static openpal::RSlice verify_hmac_sha256_trunc16(
			const SymmetricKey& key,
			const openpal::RSlice& msg,
			const openpal::RSlice& payload,
			openpal::WSlice dest,
			std::error_code& ec
		);

	};
	
}

#endif
