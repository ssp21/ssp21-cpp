
#include "ssp21/crypto/Initiator.h"

#include "ssp21/crypto/InitiatorHandshakeStates.h"

#include "ssp21/stack/LogLevels.h"

using namespace openpal;

namespace ssp21
{
    Initiator::Initiator(
        const InitiatorConfig& config,
        const Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<IExecutor>& executor,
        const Keys& keys
    ) :
        CryptoLayer(
            HandshakeMode::Initiator,
            config.config,
            config.session,
            logger,
            frame_writer,
            executor,
            keys
        ),
        handshake_state(InitiatorHandshake::Idle::get()),
        suite(config.suite),
        params(config.params),
        response_and_retry_timer(executor),
        session_timeout_timer(executor)
    {}

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_message(Initiator& ctx, const ReplyHandshakeAuth& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const Timestamp& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_response_timeout(Initiator& ctx)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::warn, "Unxpected response timeout event");
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_retry_timeout(Initiator& ctx)
    {
        SIMPLE_LOG_BLOCK(ctx.logger, levels::warn, "Unxpected retry timeout event");
        return this;
    }

    void Initiator::IHandshakeState::log_unexpected_message(Logger& logger, Function function)
    {
        FORMAT_LOG_BLOCK(logger, levels::warn, "Received unexpected message: %s", FunctionSpec::to_string(function));
    }

    void Initiator::start_response_timer()
    {
        auto on_timeout = [this]()
        {
            this->handshake_state = this->handshake_state->on_response_timeout(*this);
        };

        this->response_and_retry_timer.restart(this->params.response_timeout, on_timeout);
    }

    void Initiator::start_retry_timer()
    {
        auto on_timeout = [this]()
        {
            this->handshake_state = this->handshake_state->on_retry_timeout(*this);
        };

        this->response_and_retry_timer.restart(this->params.retry_timeout, on_timeout);
    }

    void Initiator::on_handshake_required()
    {
        this->handshake_state = this->handshake_state->on_handshake_required(*this, this->executor->get_time());
    }

    void Initiator::on_lower_open_impl()
    {
        this->on_handshake_required();
    }

    void Initiator::reset_state_on_close_from_lower()
    {
        this->handshake_state = InitiatorHandshake::Idle::get();
        this->response_and_retry_timer.cancel();
        this->session_timeout_timer.cancel();
        this->handshake_required = false;
    }

    bool Initiator::supports(Function function) const
    {
        switch (function)
        {
        case(Function::reply_handshake_begin):
        case(Function::reply_handshake_auth):
        case(Function::reply_handshake_error):
        case(Function::session_data):
            return true;
        default:
            return false;
        }
    }

    void Initiator::on_session_nonce_change(uint16_t rx_nonce, uint16_t tx_nonce)
    {
        if (max(tx_nonce, rx_nonce) >= this->params.nonce_renegotiation_trigger_value)
        {
            this->session_timeout_timer.cancel();
            this->on_handshake_required();
        }
    }

    void Initiator::on_pre_tx_ready()
    {
        if (this->handshake_required)
        {
            this->on_handshake_required();
        }
    }

    void Initiator::on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

    void Initiator::on_message(const ReplyHandshakeAuth& msg, const seq32_t& raw_data, const Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

    void Initiator::on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

}


