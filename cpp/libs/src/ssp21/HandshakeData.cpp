
#include "ssp21/HandshakeData.h"

namespace ssp21
{		
	
	HandshakeData::HandshakeData(
		const KeyPair& local_static_keys,
		const HandshakeAlgorithms& algorithms
		) : algorithms_(algorithms)
	{
		local_static_keys_.clone(local_static_keys);
	}

	void HandshakeData::initialize()
	{		
		algorithms_.gen_keypair(local_ephemeral_keys_);
	}	

	void HandshakeData::set_hash(const openpal::RSlice& input)
	{		
		algorithms_.hash({ input }, handshake_hash_);
	}
	
	void HandshakeData::mix_hash(const openpal::RSlice& input)
	{
		algorithms_.hash(
			{ handshake_hash_.as_slice(), input },
			handshake_hash_
		);
	}

	void HandshakeData::derive_authentication_key(
		const openpal::RSlice& message,
		const PublicKey& pub_e_dh_key,
		const PublicKey& pub_s_dh_key,
		std::error_code& ec)
	{
		mix_hash(message);

		DHOutput dh1;
		algorithms_.dh(local_ephemeral_keys_.private_key, pub_e_dh_key, dh1, ec);
		if (ec) return;

		DHOutput dh2;
		algorithms_.dh(local_ephemeral_keys_.private_key, pub_s_dh_key, dh2, ec);
		if (ec) return;
		
		DHOutput dh3;
		algorithms_.dh(local_static_keys_.private_key, pub_e_dh_key, dh3, ec);
		if (ec) return;

		algorithms_.hkdf(
			handshake_hash_.as_slice(),
			{ dh1.as_slice(), dh2.as_slice(), dh3.as_slice() },
			handshake_hash_,
			authentication_key_
		);
	}

	void HandshakeData::derive_session_keys(SymmetricKey& rx_key, SymmetricKey& tx_key) const
	{
		algorithms_.hkdf(handshake_hash_.as_slice(), {}, rx_key, tx_key);
	}
}


