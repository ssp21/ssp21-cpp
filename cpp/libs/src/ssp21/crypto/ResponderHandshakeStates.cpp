
#include "ssp21/crypto/ResponderHandshakeStates.h"

#include "ssp21/msg/ReplyHandshakeBegin.h"

#include "openpal/logging/LogMacros.h"
#include "ssp21/LogLevels.h"


namespace ssp21
{

    HandshakeIdle HandshakeIdle::instance;

    Responder::IHandshakeState& HandshakeIdle::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeBegin& msg)
    {
        auto err = ctx.validate(msg);

		// this won't actually be used unless the session becomes initialized
		ctx.session_init_time = ctx.executor->get_time();

        if (any(err))
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "handshake error: %s", HandshakeErrorSpec::to_string(err));
            ctx.reply_with_handshake_error(err);
            return *this;
        }

        Seq8 public_ephem_dh_key(ctx.handshake.initialize());	// generate our local ephemeral keys
        ctx.handshake.set_ck(msg_bytes);						// initialize the chaining key

        // now format our response - in the future, this we'll add certificates after this call
        ReplyHandshakeBegin reply(public_ephem_dh_key);

        auto dest = ctx.tx_buffer.as_wslice();
        auto result = reply.write_msg(dest);

        if (result.is_error())
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::error, "error formatting reply: %s", FormatErrorSpec::to_string(result.err));
            return *this;
        }

        std::error_code ec;

        ctx.handshake.derive_authentication_key(
            result.written,
            ctx.local_static_key_pair->private_key,
            msg.ephemeral_public_key,
            ctx.remote_static_public_key->as_slice(),
            ec
        );

        if (ec)
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::error, "error deriving auth key: %s", ec.message().c_str());
            ctx.reply_with_handshake_error(HandshakeError::internal);
            return *this;
        }

        ctx.lower->transmit(Message(Addresses(), result.written)); // begin transmitting the response		

        return HandshakeWaitForAuth::get();
    }

    Responder::IHandshakeState& HandshakeIdle::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeAuth& msg)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::info, "no prior request_handshake_begin");

        ctx.reply_with_handshake_error(HandshakeError::no_prior_handshake_begin);

        return *this;
    }

    HandshakeWaitForAuth HandshakeWaitForAuth::instance;

    Responder::IHandshakeState& HandshakeWaitForAuth::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeBegin& msg)
    {
        // process via HandshakeIdle
        return HandshakeIdle::get().on_message(ctx, msg_bytes, msg);
    }

    Responder::IHandshakeState& HandshakeWaitForAuth::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeAuth& msg)
    {
        /// TODO, authenticate the message!
        return HandshakeIdle::get();
    }

}


