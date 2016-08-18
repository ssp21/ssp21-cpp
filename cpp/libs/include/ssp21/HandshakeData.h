
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
			const KeyPair& local_static_keys, 
			const HandshakeAlgorithms& algorithms
		);

		/// generates new ephemeral keys and resets all state
		void initialize();		

		/// calculate a new handshake hash: h = hash(message)
		void set_hash(const openpal::RSlice& message);		

		/// derive the authentication key from the DH keys and the handshake_hash_
		void derive_authentication_key(
			const openpal::RSlice& message,
			const PublicKey& pub_e_dh_key,
			const PublicKey& pub_s_dh_key,
			std::error_code& ec
		);
	
		private:

		/// mix the data into the handshake_hash: h = hash(h || input)
		void mix_hash(const openpal::RSlice& input);

		/// running hash value (and chaining key after the derive_keys(...) step)
		SymmetricKey handshake_hash_;
		SymmetricKey authentication_key_;

		/// ephemeral keys
		KeyPair local_ephemeral_keys_;		

		/// this only gets cleared on destruction
		KeyPair local_static_keys_;
		HandshakeAlgorithms algorithms_;
	};


	
}

#endif
