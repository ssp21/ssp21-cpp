
#include "ssp21/crypto/Handshake.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/crypto/TripleDH.h"

using namespace openpal;

namespace ssp21
{
	seq32_t Handshake::generate_ephemerals()
	{		
		this->algorithms.handshake.gen_keypair(this->local_ephemeral_keys);
		return this->local_ephemeral_keys.public_key.as_seq();
	}

	void Handshake::init_handshake_hash(const seq32_t& data)
	{
		this->algorithms.handshake.hash({ data }, this->handshake_hash);
	}

	bool Handshake::initialize_session(const StaticKeys& keys, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const Timestamp& now, Session& session)
	{
		// extract the remote public key
		seq32_t remote_public_key;
		const auto err = this->cert_handler->validate(msg.certificate_data, remote_public_key);
		if (any(err))
		{
			FORMAT_LOG_BLOCK(this->logger, levels::error, "error validating certificate data: %s", HandshakeErrorSpec::to_string(err));
			return false;
		}

		// mix the handshake hash
		const auto salt = this->mix_handshake_hash(msg_bytes);

		// perform a triple-dh
		TripleDH triple_dh;

		std::error_code ec;
		const auto input_key_material = triple_dh.compute(
			this->algorithms.handshake.dh,
			keys,
			this->local_ephemeral_keys,
			remote_public_key,
			msg.ephemeral_public_key,
			ec
		);

		if (ec) {
			FORMAT_LOG_BLOCK(this->logger, levels::warn, "Error generating input key material: %s", ec.message().c_str());
			return false;
		}

		// perform session key derivation
		SessionKeys session_keys;

		this->algorithms.handshake.kdf(
			this->handshake_hash.as_seq(),
			input_key_material,
			session_keys.tx_key,
			session_keys.rx_key
		);

		return session.initialize(
			this->algorithms.session,
			Session::Param(

			),
			session_keys
		);		
	}

    seq32_t Handshake::mix_handshake_hash(const seq32_t& input)
    {
        // h = hash(h || input)

        this->algorithms.handshake.hash(
          { this->handshake_hash.as_seq(), input },
          this->handshake_hash
        );

		return this->handshake_hash.as_seq();
    }	

	/*
    void Handshake::initialize_pending_session(
        const seq32_t& message,
        const PrivateKey& priv_s_dh_key,
        const seq32_t& pub_e_dh_key,
        const seq32_t& pub_s_dh_key,
        std::error_code& ec)
    {
        this->mix_handshake_hash(message);

        DHOutput dh1;
		DHOutput dh2;
		DHOutput dh3;

        this->algorithms.handshake.dh(this->local_ephemeral_keys.private_key, pub_e_dh_key, dh1, ec);
        if (ec) return;               

        this->algorithms.handshake.dh(this->local_ephemeral_keys.private_key, pub_s_dh_key, dh2, ec);
        if (ec) return;

        this->algorithms.handshake.dh(priv_s_dh_key, pub_e_dh_key, dh3, ec);
        if (ec) return;

		SessionKeys keys;

        this->algorithms.handshake.kdf(
            this->handshake_hash.as_seq(),
			{
				dh1.as_seq(),
				// dh2 and dh3 are flipped for the initiator and the responder
				dh3.as_seq(),
				dh2.as_seq()
			},
			keys.tx_key,
			keys.rx_key
        );
		
    }

    void Handshake::initialize_session(Session& session, const openpal::Timestamp& session_start) const
    {
		/*
		TODO

        SessionKeys keys;

        // keys are swapped for initiator vs responder
        if (this->mode == HandshakeMode::Initiator)
        {
            this->algorithms.handshake.kdf(this->chaining_key.as_seq(), {}, keys.tx_key, keys.rx_key);
        }
        else
        {
            this->algorithms.handshake.kdf(this->chaining_key.as_seq(), {}, keys.rx_key, keys.tx_key);
        }

        session.initialize(
            this->algorithms.session,
            Session::Param(
                session_start,
                this->constraints.max_nonce,
                this->constraints.max_session_duration
            ),
            keys
        );		
    }
	*/

}


