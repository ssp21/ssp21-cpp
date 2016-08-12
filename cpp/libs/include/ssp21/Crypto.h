
#ifndef SSP21_CRYTPTO_H
#define SSP21_CRYTPTO_H

#include "ICryptoBackend.h"

#include <openpal/util/Uncopyable.h>

namespace ssp21
{					
	/**
	* Assumming for the time being that a static backend is fine 
	* to keep dependency injection simple.
	*/
	class Crypto final : openpal::StaticOnly
	{

	public:


		// --- These are the static proxy functions that SSP21 calls at runtime ---

		static void zero_memory(openpal::WSlice data);

		static bool secure_equals(const openpal::RSlice& lhs, const openpal::RSlice& rhs);
		
		static void hash_sha256(
				std::initializer_list<openpal::RSlice> data,
				SymmetricKey& output
		);

		static void hmac_sha256(
				const openpal::RSlice &key,
				std::initializer_list<openpal::RSlice> data,
				SymmetricKey& output
		);

		static void gen_keypair_x25519(KeyPair& pair);

		static void dh_x25519(
			const PrivateKey& priv_key,
			const PublicKey& pub_key,
			DHOutput& output,
			std::error_code& ec
		);

		// --- The HKDF function defined in Noise and SSP21, concrete implementation is here ---		

		static void hkdf_sha256(			
			const openpal::RSlice& chaining_key,
			std::initializer_list<openpal::RSlice> input_key_material,
			SymmetricKey& key1,
			SymmetricKey& key2
		);
		
		/**
		* Called once by a concrete backend during program initialization
		*/
		static void inititalize(ICryptoBackend& backend);


	private:

		typedef void(*hmac_func_t)(
			const openpal::RSlice& key,
			std::initializer_list<openpal::RSlice> data,
			SymmetricKey& output
		);

		static void hkdf(
			hmac_func_t hmac,
			const openpal::RSlice& chaining_key,
			std::initializer_list<openpal::RSlice> input_key_material,
			SymmetricKey& key1,
			SymmetricKey& key2
		);

		/// Function typedefs all initialized to nullptr. Concrete backends will initialize these.		

		static ICryptoBackend* backend_;
		
	};
}

#endif
