
#include "ssp21/crypto/Handshake.h"

using namespace openpal;

namespace ssp21
{
    HandshakeError Handshake::set_algorithms(const Algorithms::Config& config)
    {
        return this->algorithms.configure(config);
    }

    openpal::RSlice Handshake::initialize()
    {
        this->algorithms.handshake.gen_keypair(this->local_ephemeral_keys);

        return this->local_ephemeral_keys.public_key.as_slice();
    }

    void Handshake::set_ck(const RSlice& input)
    {
        this->algorithms.handshake.hash({ input }, this->chaining_key);
    }

    void Handshake::mix_ck(const RSlice& input)
    {
        // ck = hash(ck || input)

        this->algorithms.handshake.hash(
        { this->chaining_key.as_slice(), input },
        this->chaining_key
        );
    }

    bool Handshake::auth_handshake(const openpal::RSlice& mac) const
    {
        return this->algorithms.handshake.auth_handshake(this->authentication_key, this->id, mac);
    }

    void Handshake::calc_auth_handshake_reply_mac(HashOutput& output) const
    {
        return this->algorithms.handshake.calc_handshake_mac(this->authentication_key, this->id, output);
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
        this->algorithms.handshake.dh(this->local_ephemeral_keys.private_key, pub_e_dh_key, dh1, ec);
        if (ec) return;

        DHOutput dh2;
        this->algorithms.handshake.dh(this->local_ephemeral_keys.private_key, pub_s_dh_key, dh2, ec);
        if (ec) return;

        DHOutput dh3;
        this->algorithms.handshake.dh(priv_s_dh_key, pub_e_dh_key, dh3, ec);
        if (ec) return;

        this->algorithms.handshake.hkdf(
            this->chaining_key.as_slice(),
        { dh1.as_slice(), dh2.as_slice(), dh3.as_slice() },
        this->chaining_key,
        this->authentication_key
        );
    }

    void Handshake::initialize_session(Session& session, const openpal::Timestamp& session_init_time) const
    {
        auto init_keys = [this](SessionKeys & keys) -> void
        {

            // which key is which depends on initiator vs responder
            auto& tx_key = (this->id == EntityId::Initiator) ? keys.tx_key : keys.rx_key;
            auto& rx_key = (this->id == EntityId::Initiator) ? keys.rx_key : keys.tx_key;

            this->algorithms.handshake.hkdf(this->chaining_key.as_slice(), {}, tx_key, rx_key);
        };

        session.initialize(this->algorithms.session, session_init_time, init_keys);
    }

}


