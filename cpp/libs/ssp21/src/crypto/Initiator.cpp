
#include "crypto/Initiator.h"

#include "crypto/InitiatorHandshakeStates.h"

#include "ssp21/stack/LogLevels.h"

#include "log4cpp/LogMacros.h"

#include <algorithm>

namespace ssp21
{
    Initiator::Initiator(
        const InitiatorConfig& config,
        const log4cpp::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<exe4cpp::IExecutor>& executor,
        const std::shared_ptr<IInitiatorHandshake>& handshake
    ) :
        CryptoLayer(
            config.config,
            config.session,
            logger,
            frame_writer,
            executor
        ),
        params(config.params),
        session_limits(config.session_limits),
        handshake_state(InitiatorHandshakeStates::Idle::get()),
        handshake(handshake),
        response_and_retry_timer(nullptr),
        session_timeout_timer(nullptr)
    {}

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_reply_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_error_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_auth_message(Initiator& ctx, const SessionData& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now)
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

    void Initiator::IHandshakeState::log_unexpected_message(log4cpp::Logger& logger, Function function)
    {
        FORMAT_LOG_BLOCK(logger, levels::warn, "Received unexpected message: %s", FunctionSpec::to_string(function));
    }

    void Initiator::start_response_timer()
    {
        auto on_timeout = [this]()
        {
            this->handshake_state = this->handshake_state->on_response_timeout(*this);
        };

        this->response_and_retry_timer = exe4cpp::Timer(executor->start(this->params.response_timeout, on_timeout));
    }

    void Initiator::start_retry_timer()
    {
        auto on_timeout = [this]()
        {
            this->handshake_state = this->handshake_state->on_retry_timeout(*this);
            this->on_handshake_required();
        };

        this->response_and_retry_timer = exe4cpp::Timer(executor->start(this->params.retry_timeout, on_timeout));
    }

    void Initiator::start_session_timer(const exe4cpp::steady_time_t& session_timeout)
    {
        auto on_timeout = [this]()
        {
            this->on_handshake_required();
        };

        this->session_timeout_timer = exe4cpp::Timer(executor->start(session_timeout, on_timeout));
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
        this->handshake_state = InitiatorHandshakeStates::Idle::get();
        this->response_and_retry_timer.cancel();
        this->session_timeout_timer.cancel();
        this->handshake_required = false;
    }

    bool Initiator::supports(Function function) const
    {
        switch (function)
        {
        case(Function::reply_handshake_begin):
        case(Function::reply_handshake_error):
        case(Function::session_data):
            return true;
        default:
            return false;
        }
    }

    void Initiator::on_session_nonce_change(uint16_t rx_nonce, uint16_t tx_nonce)
    {
        if (ser4cpp::max(tx_nonce, rx_nonce) >= this->params.nonce_renegotiation_trigger_value)
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

    void Initiator::on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
    {
        this->handshake_state = this->handshake_state->on_reply_message(*this, msg, raw_data, now);
    }

    void Initiator::on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
    {
        this->handshake_state = this->handshake_state->on_error_message(*this, msg, raw_data, now);
    }

    void Initiator::on_auth_session(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
    {
        this->handshake_state = this->handshake_state->on_auth_message(*this, msg, raw_data, now);
    }

}
