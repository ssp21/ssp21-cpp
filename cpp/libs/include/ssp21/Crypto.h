
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include <openpal/container/RSlice.h>

#include <initializer_list>
#include <system_error>

namespace ssp21
{	
	typedef openpal::RSlice(*HMACFuncT)(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest,
		std::error_code& ec
	);

	typedef openpal::RSlice(*HASHFuncT)(		
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest,
		std::error_code& ec
	);

	class Crypto
	{

	public:
		
		/// Function typedefs all initialized to nullptr. Concrete backends will initialize these.

		static HMACFuncT HMAC_SHA256;

		static HASHFuncT HASH_SHA256;


	private:
		Crypto() = delete;
		Crypto(Crypto&) = delete;
	};
}

#endif
