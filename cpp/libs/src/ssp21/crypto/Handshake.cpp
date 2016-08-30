
#include "ssp21/crypto/Handshake.h"

using namespace openpal;

namespace ssp21
{   
	HandshakeError Handshake::set_algorithms(DHMode dh_mode, HashMode hash_mode, NonceMode nonce_mode)
	{
		return this->algorithms_.configure(dh_mode, hash_mode, nonce_mode);
	}

	openpal::RSlice Handshake::initialize()
    {
        algorithms_.gen_keypair(local_ephemeral_keys_);

		return local_ephemeral_keys_.public_key.as_slice();
    }

    void Handshake::set_ck(const RSlice& input)
    {
        algorithms_.hash({ input }, chaining_key_);
    }

	void Handshake::mix_ck(const RSlice& input)
	{
		// ck = hash(ck || input)

		algorithms_.hash(
		{ chaining_key_.as_slice(), input },
			chaining_key_
		);
	}	

    void Handshake::derive_authentication_key(
        const RSlice& message,
        const PrivateKey& priv_s_dh_key,
        const RSlice& pub_e_dh_key,
        const RSlice& pub_s_dh_key,
        std::error_code& ec)
    {        
		this->mix_ck(message);

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
            chaining_key_.as_slice(),
        { dh1.as_slice(), dh2.as_slice(), dh3.as_slice() },
        chaining_key_,
        authentication_key_
        );
    }

    openpal::RSlice Handshake::get_auth_key() const
    {
        return authentication_key_.as_slice();
    }

    void Handshake::derive_session_keys(SymmetricKey& rx_key, SymmetricKey& tx_key) const
    {
        algorithms_.hkdf(chaining_key_.as_slice(), {}, rx_key, tx_key);
    }
}


