
#ifndef SSP21_HANDSHAKE_DATA_H
#define SSP21_HANDSHAKE_DATA_H

#include "ssp21/HandshakeAlgorithms.h"

namespace ssp21
{		
	
	/**
	* Class that holds all of the data during the handshake process
	*/
	class HandshakeData : private openpal::Uncopyable
	{
		public:	

		HandshakeData(			
			const HandshakeAlgorithms& algorithms
		);

		/// generates new ephemeral keys and resets all state
		void initialize();		

		/// calculate a new handshake hash: h = hash(message)
		void set_hash(const openpal::RSlice& message);		

		/// derive the authentication key from the DH keys and the handshake_hash_
		void derive_authentication_key(
			const openpal::RSlice& message,
			const PrivateKey& priv_s_dh_key,
			const PublicKey& pub_e_dh_key,
			const PublicKey& pub_s_dh_key,
			std::error_code& ec
		);

		/// derive the session keys
		void derive_session_keys(SymmetricKey& rx_key, SymmetricKey& tx_key) const;
	
		private:

		/// mix the data into the handshake_hash: h = hash(h || input)
		void mix_hash(const openpal::RSlice& input);		

		/// specific algorithms used to perform steps
		HandshakeAlgorithms algorithms_;		

		/// running hash value (and chaining key after the derive_authentication_key(...) step)
		SymmetricKey handshake_hash_;

		/// authentication key derived during derive_authentication_key(...)
		SymmetricKey authentication_key_;

		/// ephemeral keys
		KeyPair local_ephemeral_keys_;				
	};


	
}

#endif
