
#ifndef SSP21_HANDSHAKE_H
#define SSP21_HANDSHAKE_H

#include "CryptoTypedefs.h"

namespace ssp21
{		
	
	/**
	* Class that holds all of the data during the handshake process
	*/
	class Handshake : private openpal::Uncopyable
	{
		public:

		struct Algorithms
		{

		public:

			Algorithms(dh_func_t dh, hkdf_func_t hkdf, hash_func_t hash, gen_keypair_func_t gen_keypair) :
				dh(dh),
				hkdf(hkdf),
				hash(hash),
				gen_keypair(gen_keypair)
			{}

			dh_func_t dh;
			hkdf_func_t hkdf;
			hash_func_t hash;
			gen_keypair_func_t gen_keypair;
		};

		Handshake(const Algorithms& algorithms);

		/// generates new ephemeral keys and resets all state
		void initialize();		

		/// calculate a new handshake hash: h = hash(message)
		void set_hash(const openpal::RSlice& message);		

		/// derive the authentication key from the DH keys and the handshake_hash_
		void derive_authentication_key(
			const openpal::RSlice& message,
			const PrivateKey& priv_s_dh_key,
			const openpal::RSlice& pub_e_dh_key,
			const openpal::RSlice& pub_s_dh_key,
			std::error_code& ec
		);

		/// return a slice pointing to the authentication key
		openpal::RSlice get_auth_key() const;

		/// derive the session keys
		void derive_session_keys(SymmetricKey& rx_key, SymmetricKey& tx_key) const;		
	
		private:		

		/// specific algorithms used to perform steps
		Algorithms algorithms_;

		/// running hash value and chaining key after the derive_authentication_key(...) step
		SymmetricKey chaining_key_;

		/// authentication key derived during derive_authentication_key(...)
		SymmetricKey authentication_key_;

		/// ephemeral keys
		KeyPair local_ephemeral_keys_;				
	};


	
}

#endif
