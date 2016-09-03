
#include "ssp21/crypto/Handshake.h"

using namespace openpal;

namespace ssp21
{
    HandshakeError Handshake::set_algorithms(const Algorithms::Config& config)
    {
        return this->algorithms_.configure(config);
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

    bool Handshake::auth_handshake(const openpal::RSlice& mac) const
    {
        return algorithms_.auth_handshake(authentication_key_, id_, mac);
    }

    void Handshake::calc_auth_handshake_reply_mac(HashOutput& output) const
    {
        return algorithms_.calc_handshake_mac(authentication_key_, id_, output);
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

    void Handshake::initialize_session(Session& session, const openpal::Timestamp& session_init_time) const
    {
        auto init_keys = [this](SessionKeys & keys) -> void
        {

            // which key is which depends on initiator vs responder
            auto& tx_key = (this->id_ == EntityId::Initiator) ? keys.tx_key : keys.rx_key;
            auto& rx_key = (this->id_ == EntityId::Initiator) ? keys.rx_key : keys.tx_key;

            this->algorithms_.hkdf(chaining_key_.as_slice(), {}, tx_key, rx_key);
        };

        session.initialize(this->algorithms_.session, session_init_time, init_keys);
    }

}


