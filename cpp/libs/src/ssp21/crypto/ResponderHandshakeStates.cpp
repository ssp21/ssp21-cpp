
#include "ssp21/crypto/ResponderHandshakeStates.h"

#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeAuth.h"

#include "openpal/logging/LogMacros.h"
#include "ssp21/LogLevels.h"


namespace ssp21
{

    // -------------------------- HandshakeIdle -----------------------------

    Responder::IHandshakeState* ResponderHandshakeIdle::on_message(Responder& ctx, const RequestHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        auto err = ctx.configure_feature_support(msg);

        if (any(err))
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "handshake error: %s", HandshakeErrorSpec::to_string(err));
            ctx.reply_with_handshake_error(err);
            return this;
        }

        seq8_t public_ephem_dh_key(ctx.handshake.initialize());	// generate our local ephemeral keys
        ctx.handshake.set_ck(msg_bytes);						// initialize the chaining key

        // now format our response - in the future, this we'll add certificates after this call if applicable
        ReplyHandshakeBegin reply(public_ephem_dh_key);

        const auto res = ctx.frame_writer->write(reply);

        if (res.is_error())
        {
            return this;
        }

        std::error_code ec;

        ctx.handshake.derive_authentication_key(
            res.written,
            ctx.local_static_key_pair->private_key,
            msg.ephemeral_public_key,
            ctx.remote_static_public_key->as_seq(),
            ec
        );

        if (ec)
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::error, "error deriving auth key: %s", ec.message().c_str());
            ctx.reply_with_handshake_error(HandshakeError::internal);
            return this;
        }

        ctx.lower->transmit(res.frame);

        return ResponderHandshakeWaitForAuth::get();
    }

    Responder::IHandshakeState* ResponderHandshakeIdle::on_message(Responder& ctx, const RequestHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::info, "no prior request_handshake_begin");

        ctx.reply_with_handshake_error(HandshakeError::no_prior_handshake_begin);

        return this;
    }

    // -------------------------- HandshakeWaitForAuth -----------------------------

    Responder::IHandshakeState* ResponderHandshakeWaitForAuth::on_message(Responder& ctx, const RequestHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        // process via HandshakeIdle
        return ResponderHandshakeIdle::get()->on_message(ctx, msg, msg_bytes, now);
    }

    Responder::IHandshakeState* ResponderHandshakeWaitForAuth::on_message(Responder& ctx, const RequestHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        if (!ctx.handshake.auth_handshake(msg.mac)) // auth success
        {
            SIMPLE_LOG_BLOCK(ctx.logger, levels::warn, "RequestHandshakeAuth: authentication failure");
            ctx.reply_with_handshake_error(HandshakeError::authentication_error);
            return ResponderHandshakeIdle::get();
        }

        ctx.handshake.mix_ck(msg_bytes);

        HashOutput reply_mac;
        ctx.handshake.calc_auth_handshake_reply_mac(reply_mac);

        ReplyHandshakeAuth reply(seq8_t(reply_mac.as_seq()));

        const auto res = ctx.frame_writer->write(reply);

        if (res.is_error())
        {
            return ResponderHandshakeIdle::get();
        }

        ctx.handshake.mix_ck(res.written);

        ctx.handshake.initialize_session(ctx.session, now);

        ctx.lower->transmit(res.frame);

        ctx.upper->on_open();

        return ResponderHandshakeIdle::get();
    }

}


