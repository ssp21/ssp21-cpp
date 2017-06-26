
#include "ssp21/crypto/InitiatorHandshakeStates.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/stack/LogLevels.h"

using namespace openpal;

namespace ssp21
{

    // -------- Idle --------

    Initiator::IHandshakeState* InitiatorHandshake::Idle::on_handshake_required(Initiator& ctx, const Timestamp& now)
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
            ctx.suite.handshake_mac,
            ctx.suite.session_mode
        );

        const auto err = ctx.handshake.set_algorithms(crypto_spec);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::error, "Error configuring handshake algorithms: %s", HandshakeErrorSpec::to_string(err));
            // There's nothing we can do about a bad configuration, so just go to an error state
            return InitiatorHandshake::BadConfiguration::get();
        }

        const auto public_key = ctx.handshake.initialize();

        const RequestHandshakeBegin request(
            consts::crypto::protocol_version,
            crypto_spec,
            SessionConstraints(
                ctx.params.max_nonce_value,
                ctx.params.max_session_time_ms
            ),
            ctx.certificate_handler->mode(),
            public_key,
            ctx.certificate_handler->certificate_data()
        );

        const auto result = ctx.frame_writer->write(request);

        if (result.is_error())
        {
            return InitiatorHandshake::BadConfiguration::get();
        }

        ctx.handshake.begin_handshake(request, result.written);

        ctx.lower->start_tx_from_upper(result.frame);

        // record when we transmited the request so we can estimate the time base later
        ctx.request_handshake_begin_time_tx = ctx.executor->get_time();

        ctx.start_response_timer();

        return InitiatorHandshake::WaitForBeginReply::get();
    }

    // -------- WaitForBeginReply --------

    Initiator::IHandshakeState* InitiatorHandshake::WaitForBeginReply::on_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        ctx.response_and_retry_timer.cancel();

        seq32_t remote_public_key;
        const auto err = ctx.certificate_handler->validate(msg.certificate_data, remote_public_key);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::error, "error validating certificate data: %s", HandshakeErrorSpec::to_string(err));
            ctx.start_retry_timer();
            return WaitForRetry::get();
        }

        std::error_code ec;

        ctx.handshake.derive_authentication_key(
            msg_bytes,
            *ctx.keys.private_key,
            msg.ephemeral_public_key,
            remote_public_key,
            ec
        );

        if (ec)
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::error, "error deriving authentication key: %s", ec.message().c_str());
            ctx.start_retry_timer();
            return WaitForRetry::get();
        }

        HashOutput hash;
        ctx.handshake.calc_auth_handshake_mac(hash);

        const RequestHandshakeAuth request(hash.as_seq());

        const auto result = ctx.frame_writer->write(request);

        if (result.is_error())
        {
            return InitiatorHandshake::BadConfiguration::get();
        }

        ctx.handshake.mix_ck(result.written);

        ctx.lower->start_tx_from_upper(result.frame);

        ctx.start_response_timer();

        return WaitForAuthReply::get();
    }

    Initiator::IHandshakeState* InitiatorHandshake::WaitForBeginReply::on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        ctx.response_and_retry_timer.cancel();
        FORMAT_LOG_BLOCK(ctx.logger, levels::error, "responder handshake error: %s", HandshakeErrorSpec::to_string(msg.handshake_error));
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    Initiator::IHandshakeState* InitiatorHandshake::WaitForBeginReply::on_response_timeout(Initiator& ctx)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::error, "timeout while waiting for ReplyHandshakeBegin");
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    // -------- WaitForAuthReply --------

    Initiator::IHandshakeState* InitiatorHandshake::WaitForAuthReply::on_message(Initiator& ctx, const ReplyHandshakeAuth& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        ctx.response_and_retry_timer.cancel();

        if (!ctx.handshake.auth_handshake(msg.mac))
        {
            SIMPLE_LOG_BLOCK(ctx.logger, levels::warn, "authentication failure");
            ctx.start_retry_timer();
            return WaitForRetry::get();
        }

        ctx.handshake.mix_ck(msg_bytes);

        if (now.milliseconds < ctx.request_handshake_begin_time_tx.milliseconds)
        {
            SIMPLE_LOG_BLOCK(ctx.logger, levels::error, "clock rollback detected");
            ctx.start_retry_timer();
            return WaitForRetry::get();
        }

        const auto elapsed_ms = now.milliseconds - ctx.request_handshake_begin_time_tx.milliseconds;
        const auto session_init_time = now.milliseconds - (elapsed_ms / 2); // estimate

        ctx.handshake.initialize_session(ctx.session, Timestamp(session_init_time));

        ctx.handshake_required = false;

        // the absolute time at which a renegotation should be triggered
        const Timestamp session_timeout_abs_time(session_init_time + ctx.params.session_time_renegotiation_trigger_ms);

        ctx.session_timeout_timer.restart(session_timeout_abs_time, [&ctx]()
        {
            ctx.on_handshake_required();
        });

        ctx.upper->on_lower_open();

        return Idle::get();
    }

    Initiator::IHandshakeState* InitiatorHandshake::WaitForAuthReply::on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        ctx.response_and_retry_timer.cancel();
        FORMAT_LOG_BLOCK(ctx.logger, levels::error, "responder handshake error: %s", HandshakeErrorSpec::to_string(msg.handshake_error));
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    Initiator::IHandshakeState* InitiatorHandshake::WaitForAuthReply::on_response_timeout(Initiator& ctx)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::error, "timeout while waiting for ReplyHandshakeAuth");
        ctx.start_retry_timer();
        return WaitForRetry::get();
    }

    // -------- WaitForRetry --------

    Initiator::IHandshakeState* InitiatorHandshake::WaitForRetry::on_retry_timeout(Initiator& ctx)
    {
        return Idle::get();
    }
}


