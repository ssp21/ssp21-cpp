
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include <openpal/container/RSlice.h>
#include <openpal/container/WSlice.h>

#include <initializer_list>
#include <system_error>

namespace ssp21
{		
	typedef openpal::RSlice(*HASHFuncT)(		
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest		
	);

	typedef openpal::RSlice(*HMACFuncT)(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest
		);

	class Crypto
	{

	public:

		static const uint8_t SHA256_HASH_OUTPUT_LENGTH = 32;

		static openpal::RSlice CalcHash_SHA256(
			std::initializer_list<openpal::RSlice> data,
			openpal::WSlice& dest);

		static openpal::RSlice CalcHMAC_SHA256(
			const openpal::RSlice& key,
			std::initializer_list<openpal::RSlice> data,
			openpal::WSlice& dest);

		
		static void Inititalize(
			HASHFuncT hash_sha256,
			HMACFuncT hmac_sha256			
		);


	private:

		/// Function typedefs all initialized to nullptr. Concrete backends will initialize these.
		/// These should not be used directly

		static HASHFuncT m_hash_sha256;
		static HMACFuncT m_hmac_sha256;
		

		Crypto() = delete;
		Crypto(Crypto&) = delete;
	};
}

#endif
