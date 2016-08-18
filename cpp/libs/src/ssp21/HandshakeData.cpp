
#include "ssp21/HandshakeData.h"

using namespace openpal;

namespace ssp21
{		
	
	HandshakeData::HandshakeData(const HandshakeAlgorithms& algorithms) : algorithms_(algorithms)
	{
		
	}

	void HandshakeData::initialize()
	{		
		algorithms_.gen_keypair(local_ephemeral_keys_);
	}	

	void HandshakeData::set_hash(const RSlice& input)
	{		
		algorithms_.hash({ input }, handshake_hash_);
	}	

	void HandshakeData::derive_authentication_key(
		const RSlice& message,
		const PrivateKey& priv_s_dh_key,
		const RSlice& pub_e_dh_key,
		const RSlice& pub_s_dh_key,
		std::error_code& ec)
	{
		// mix the hash: h = hash(h || input)
		algorithms_.hash(
			{ handshake_hash_.as_slice(), message },
			handshake_hash_
		);

		DHOutput dh1;
		algorithms_.dh(local_ephemeral_keys_.private_key, pub_e_dh_key, dh1, ec);
		if (ec) return;

		DHOutput dh2;
		algorithms_.dh(local_ephemeral_keys_.private_key, pub_s_dh_key, dh2, ec);
		if (ec) return;
		
		DHOutput dh3;
		algorithms_.dh(priv_s_dh_key, pub_e_dh_key, dh3, ec);
		if (ec) return;

		algorithms_.hkdf(
			handshake_hash_.as_slice(),
			{ dh1.as_slice(), dh2.as_slice(), dh3.as_slice() },
			handshake_hash_,
			authentication_key_
		);		
	}

	openpal::RSlice HandshakeData::get_auth_key() const
	{
		return authentication_key_.as_slice();
	}

	void HandshakeData::derive_session_keys(SymmetricKey& rx_key, SymmetricKey& tx_key) const
	{
		algorithms_.hkdf(handshake_hash_.as_slice(), {}, rx_key, tx_key);
	}
}


