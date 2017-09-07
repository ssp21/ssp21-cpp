
#include "ssp21/crypto/InitiatorHandshakeStates.h"

#include "ssp21/crypto/TripleDH.h"
#include "ssp21/stack/LogLevels.h"

#include "openpal/logging/LogMacros.h"

using namespace openpal;

namespace ssp21
{

    // -------- Idle --------

    Initiator::IHandshakeState* InitiatorHandshakeStates::Idle::on_handshake_required(Initiator& ctx, const Timestamp& now)
    {
        if (!ctx.lower->is_tx_ready())
        {
            ctx.handshake_required = true;
            return this;
        }

        const CryptoSpec crypto_spec(
            ctx.suite.nonce_mode,
            ctx.suite.dh_mode,
            ctx.suite.handshake_hash,
            ctx.suite.handshake_kdf,
            ctx.suite.session_mode
        );

        const auto ephemeral_data = ctx.handshake.generate_ephemeral_data();

        const RequestHandshakeBegin request(
            consts::crypto::protocol_version,
            crypto_spec,
            SessionConstraints(
                ctx.params.max_nonce_value,
                ctx.params.max_session_time_ms
            ),
            ctx.handshake.get_certificate_mode(),
			ephemeral_data,
            ctx.handshake.get_mode_data()
        );

        const auto result = ctx.frame_writer->write(request);

        if (result.is_error())
        {
            return InitiatorHandshakeStates::BadConfiguration::get();
        }

        ctx.handshake.begin_request_transmit(result.written, now);

        ctx.lower->start_tx_from_upper(result.frame);        

        ctx.start_response_timer();

        return InitiatorHandshakeStates::WaitForBeginReply::get();
    }

    // -------- WaitForBeginReply --------

    Initiator::IHandshakeState* InitiatorHandshakeStates::WaitForBeginReply::on_reply_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        ctx.response_and_retry_timer.cancel();

		if (!ctx.handshake.initialize_session(msg, msg_bytes, ctx.params, now, *ctx.sessions.pending))
		{
			ctx.start_retry_timer();
			return WaitForRetry::get();
		}
		
		if (!ctx.transmit_session_auth(*ctx.sessions.pending))
		{
			ctx.start_retry_timer();
			return WaitForRetry::get();
		}
		
		ctx.start_response_timer();		

        return WaitForAuthReply::get();
    }

    Initiator::IHandshakeState* InitiatorHandshakeStates::WaitForBeginReply::on_error_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        ctx.response_and_retry_timer.cancel();
        FORMAT_LOG_BLOCK(ctx.logger, levels::error, "responder handshake error: %s", HandshakeErrorSpec::to_string(msg.handshake_error));
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    Initiator::IHandshakeState* InitiatorHandshakeStates::WaitForBeginReply::on_response_timeout(Initiator& ctx)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::error, "timeout while waiting for ReplyHandshakeBegin");
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    // -------- WaitForAuthReply --------

	Initiator::IHandshakeState* InitiatorHandshakeStates::WaitForAuthReply::on_auth_message(Initiator& ctx, const SessionData& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
	{		
        ctx.response_and_retry_timer.cancel();

		std::error_code ec;
		const auto payload = ctx.sessions.pending->validate_session_auth(msg, now, ctx.payload_buffer.as_wslice(), ec);

		if (ec)
		{
			FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "Error validating session auth: %s", ec.message().c_str());
			ctx.start_retry_timer();
			return WaitForRetry::get();
		}

		// go ahead and activate the session
		ctx.sessions.activate_pending();

		// we've completed the handshake
		ctx.handshake_required = false;
		
		// the absolute time at which a renegotation should be triggered
		const Timestamp session_timeout_abs_time(ctx.sessions.active->get_session_start().milliseconds + ctx.params.session_time_renegotiation_trigger_ms);

		ctx.session_timeout_timer.restart(session_timeout_abs_time, [&ctx]()
		{
			ctx.on_handshake_required();
		});

		ctx.upper->on_lower_open();

        if (payload.is_not_empty())
        {
            // process the payload 
			ctx.payload_data = payload;
			ctx.upper->on_lower_rx_ready();
        }		       
       
        return Idle::get();
    }
    

    Initiator::IHandshakeState* InitiatorHandshakeStates::WaitForAuthReply::on_error_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        ctx.response_and_retry_timer.cancel();
        FORMAT_LOG_BLOCK(ctx.logger, levels::error, "responder handshake error: %s", HandshakeErrorSpec::to_string(msg.handshake_error));
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    Initiator::IHandshakeState* InitiatorHandshakeStates::WaitForAuthReply::on_response_timeout(Initiator& ctx)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::error, "timeout while waiting for ReplyHandshakeAuth");
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    // -------- WaitForRetry --------

    Initiator::IHandshakeState* InitiatorHandshakeStates::WaitForRetry::on_retry_timeout(Initiator& ctx)
    {
        return Idle::get();
    }
}


