
#include "ssp21/crypto/Handshake.h"

using namespace openpal;

namespace ssp21
{
    HandshakeError Handshake::set_algorithms(const CryptoSpec& spec)
    {
        return this->algorithms.configure(spec);
    }

    seq32_t Handshake::initialize()
    {
        this->algorithms.handshake.gen_keypair(this->local_ephemeral_keys);

        return this->local_ephemeral_keys.public_key.as_seq();
    }

    void Handshake::begin_handshake(const RequestHandshakeBegin& msg, const seq32_t& raw_msg)
    {
        this->constraints = msg.constraints;

        this->algorithms.handshake.hash({ raw_msg }, this->chaining_key);
    }

    void Handshake::mix_ck(const seq32_t& input)
    {
        // ck = hash(ck || input)

        this->algorithms.handshake.hash(
        { this->chaining_key.as_seq(), input },
        this->chaining_key
        );
    }

    bool Handshake::auth_handshake(const seq32_t& mac) const
    {
		// TODO
		return false;
    }

    void Handshake::calc_auth_handshake_mac(HashOutput& output) const
    {
        // TODO
    }

    void Handshake::derive_authentication_key(
        const seq32_t& message,
        const PrivateKey& priv_s_dh_key,
        const seq32_t& pub_e_dh_key,
        const seq32_t& pub_s_dh_key,
        std::error_code& ec)
    {
        this->mix_ck(message);

        DHOutput dh1;
        this->algorithms.handshake.dh(this->local_ephemeral_keys.private_key, pub_e_dh_key, dh1, ec);
        if (ec) return;

        // dh2 and dh3 are flipped for the initiator and the responder
        DHOutput dh2;
        DHOutput dh3;

        this->algorithms.handshake.dh(this->local_ephemeral_keys.private_key, pub_s_dh_key, (this->mode == HandshakeMode::Responder) ? dh2 : dh3, ec);
        if (ec) return;

        this->algorithms.handshake.dh(priv_s_dh_key, pub_e_dh_key, (this->mode == HandshakeMode::Responder) ? dh3 : dh2, ec);
        if (ec) return;

        this->algorithms.handshake.kdf(
            this->chaining_key.as_seq(),
        {
            dh1.as_seq(), dh2.as_seq(), dh3.as_seq()
        },
        this->chaining_key,
        this->authentication_key
        );
    }

    void Handshake::initialize_session(Session& session, const openpal::Timestamp& session_start) const
    {
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

}


