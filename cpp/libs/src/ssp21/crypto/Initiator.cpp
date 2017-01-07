
#include "ssp21/crypto/Initiator.h"

#include "ssp21/crypto/InitiatorHandshakeStates.h"

#include "ssp21/LogLevels.h"

namespace ssp21
{
    Initiator::Initiator(
        const Config& context_config,
        const Session::Config& session_config,
        const openpal::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<openpal::IExecutor>& executor,
        std::unique_ptr<KeyPair> local_static_key_pair,
        std::unique_ptr<PublicKey> remote_static_public_key
    ) :
        CryptoLayer(
            HandshakeMode::Initiator,
            context_config,
            session_config,
            logger,
            frame_writer,
            executor,
            std::move(local_static_key_pair),
            std::move(remote_static_public_key)
        ),
        handshake_state(InitiatorHandshakeIdle::get()),
        suite(context_config.suite),
        response_timer(*executor)
    {}

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_message(Initiator& ctx, const ReplyHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    Initiator::IHandshakeState* Initiator::IHandshakeState::on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        this->log_unexpected_message(ctx.logger, msg.function);
        return this;
    }

    void Initiator::IHandshakeState::log_unexpected_message(openpal::Logger& logger, Function function)
    {
        FORMAT_LOG_BLOCK(logger, levels::warn, "Received unexpected message: %s", FunctionSpec::to_string(function));
    }

    void Initiator::on_open_impl()
    {
        this->handshake_state = this->handshake_state->on_handshake_required(*this, this->executor->get_time());
    }

    void Initiator::reset_state_on_close()
    {
        this->handshake_state = InitiatorHandshakeIdle::get();
        this->response_timer.cancel();
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

    void Initiator::on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

    void Initiator::on_message(const ReplyHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

    void Initiator::on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

}


