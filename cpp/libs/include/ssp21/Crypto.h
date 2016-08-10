
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include "Key.h"

#include <initializer_list>
#include <system_error>

namespace ssp21
{		
	typedef void (*ZeroT)(openpal::WSlice&);

	typedef openpal::RSlice(*HASHFuncT)(		
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest
	);

	typedef openpal::RSlice(*HMACFuncT)(
		const openpal::RSlice& key,
		std::initializer_list<openpal::RSlice> data,
		openpal::WSlice& dest
	);
	
	typedef void(*GenKeyPairT)(KeyPair& pair);

	typedef void (*DH_X25519T)(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec);

	
	/**
	* Assumming for the time being that a static backend is fine 
	* to keep dependency injection simple.
	*/
	class Crypto
	{

	public:		

		static void Zero(openpal::WSlice&);
		
		static openpal::RSlice CalcHash_SHA256(
			std::initializer_list<openpal::RSlice> data,
			openpal::WSlice& dest);

		static openpal::RSlice CalcHMAC_SHA256(
			const openpal::RSlice& key,
			std::initializer_list<openpal::RSlice> data,
			openpal::WSlice& dest);

		static void GenKeyPair_X25519(KeyPair& pair);

		static void DH_X25519(const openpal::RSlice& priv_key, const openpal::RSlice& pub_key, Key& output, std::error_code& ec);
		
		static void Inititalize(
			ZeroT zero_memory,
			HASHFuncT hash_sha256,
			HMACFuncT hmac_sha256,
			GenKeyPairT genpair_x25519,
			DH_X25519T dh_x25519
		);


	private:

		/// Function typedefs all initialized to nullptr. Concrete backends will initialize these.
		/// These should not be used directly

		static ZeroT m_zero_memory;
		static HASHFuncT m_hash_sha256;
		static HMACFuncT m_hmac_sha256;

		static GenKeyPairT m_genpair_x25519;
		static DH_X25519T m_dh_x25519;
		

		Crypto() = delete;
		Crypto(Crypto&) = delete;
	};
}

#endif
