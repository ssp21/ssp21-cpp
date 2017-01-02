
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/ResponderHandshakeStates.h"

#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/MessageDispatcher.h"

using namespace openpal;

namespace ssp21
{
    Responder::Context::Context(
        const Config& config,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        std::unique_ptr<KeyPair> local_static_key_pair,
        std::unique_ptr<PublicKey> remote_static_public_key,
        const openpal::Logger& logger,
        const std::shared_ptr<openpal::IExecutor>& executor,
        ILowerLayer& lower) :

        config(config),
        frame_writer(frame_writer),
        local_static_key_pair(std::move(local_static_key_pair)),
        remote_static_public_key(std::move(remote_static_public_key)),
        logger(logger),
        executor(executor),
        handshake(EntityId::Responder),
        session(frame_writer, config.session),
        reassembler(config.max_reassembly_size),
        lower(&lower)
    {

    }

    void Responder::Context::reply_with_handshake_error(HandshakeError err)
    {
        ReplyHandshakeError msg(err);
        const auto res = this->frame_writer->write(msg);
        if (!res.is_error())
        {
            this->lower->transmit(res.frame);
        }
    }

    HandshakeError Responder::Context::validate(const RequestHandshakeBegin& msg)
    {
        if (msg.version != consts::crypto::protocol_version)
        {
            return HandshakeError::unsupported_version;
        }

        // verify that the public key length matches the DH mode
        if (msg.ephemeral_public_key.length() != consts::crypto::x25519_key_length)
        {
            return HandshakeError::bad_message_format;
        }

        if (msg.certificate_mode != CertificateMode::preshared_keys)
        {
            return HandshakeError::unsupported_certificate_mode;
        }

        if (msg.certificates.count() != 0)
        {
            return HandshakeError::bad_message_format;
        }

        // last thing we should do is configure the requested algorithms
        return handshake.set_algorithms(msg.spec);
    }

    Responder::Responder(const Config& config,
                         const std::shared_ptr<IFrameWriter>& frame_writer,
                         std::unique_ptr<KeyPair> local_static_key_pair,
                         std::unique_ptr<PublicKey> remote_static_public_key,
                         const openpal::Logger& logger,
                         const std::shared_ptr<openpal::IExecutor>& executor,
                         ILowerLayer& lower
                        ) :
        ctx(config,
            std::move(frame_writer),
            std::move(local_static_key_pair),
            std::move(remote_static_public_key),
            logger,
            executor,
            lower),
        handshake_state(&HandshakeIdle::get())
    {}

    void Responder::on_close_impl()
    {
        this->handshake_state = &HandshakeIdle::get();

        this->ctx.session.reset();
        this->ctx.reassembler.reset();
        this->ctx.upper->on_close();
        this->ctx.tx_state.reset();

        this->reset_lower_layer();
    }

    void Responder::on_tx_ready_impl()
    {
        if (ctx.tx_state.on_tx_complete())
        {
            // ready to transmit more data
            this->is_tx_ready = true;
            ctx.upper->on_tx_ready();
        }

        this->check_receive();
        this->check_transmit();
    }

    bool Responder::on_rx_ready_impl(const seq32_t& data)
    {
        if (this->can_receive())
        {
            this->process(data);
            return true;
        }
        else
        {
            return false;
        }
    }

    void Responder::check_receive()
    {
        if (this->can_receive())
        {
            ctx.lower->receive();
        }
    }

    void Responder::check_transmit()
    {
        /**
        * 1) The session must be able to transmit
        * 2) Lower-layer must be ready to transmit
        * 3) transmission state must have some data to send
        */
        if (ctx.session.can_transmit() && ctx.lower->get_is_tx_ready() && ctx.tx_state.is_ready_tx())
        {
            auto remainder = this->ctx.tx_state.get_remainder();
            const auto fir = this->ctx.tx_state.get_fir();
            const auto now = this->ctx.executor->get_time();

            std::error_code err;
            const auto data = ctx.session.format_session_message(fir, now, remainder, err);
            if (err)
            {
                FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "Error formatting session message: %s", err.message().c_str());
                return;
            }

            ctx.tx_state.begin_transmit(remainder);

            ctx.lower->transmit(data);
        }
    }

    bool Responder::transmit(const seq32_t& data)
    {
        if (!this->get_is_open())
        {
            return false;
        }

        if (!this->ctx.upper->get_is_open())
        {
            return false;
        }

        // already transmitting on behalf on the upper layer
        if (!ctx.tx_state.initialize(data))
        {
            return false;
        }

        this->check_transmit();

        return true;
    }

    void Responder::receive()
    {
        if (this->is_rx_ready && this->ctx.upper->on_rx_ready(this->ctx.reassembler.get_data()))
        {
            this->is_rx_ready = false;
        }
    }

    void Responder::process(const seq32_t& message)
    {
        MessageDispatcher::Dispatch(
            this->ctx.logger,
            message,
            this->ctx.executor->get_time(),
            *this
        );
    }

    bool Responder::supports(Function function) const
    {
        switch (function)
        {
        case(Function::request_handshake_begin):
        case(Function::request_handshake_auth):
        case(Function::session_data):
            return true;
        default:
            return false;
        }
    }

    void Responder::on_parse_error(Function function, ParseError error)
    {
        switch (function)
        {
        case(Function::request_handshake_begin):
        case(Function::request_handshake_auth):
            this->ctx.reply_with_handshake_error(HandshakeError::bad_message_format);
            break;
        default:
            break;
        }
    }

    bool Responder::on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = &this->handshake_state->on_message(ctx, raw_data, msg);
        return true;
    }

    bool Responder::on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = &this->handshake_state->on_message(ctx, raw_data, msg);
        return true;
    }

    bool Responder::on_message(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        std::error_code ec;
        const auto payload = this->ctx.session.validate_message(msg, now, ec);

        if (ec)
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "validation error: %s", ec.message().c_str());
            return false;
        }

        // process the message using the reassembler
        const auto result = this->ctx.reassembler.process(msg.metadata.flags.fir, msg.metadata.flags.fin, msg.metadata.nonce, payload);

        switch (result)
        {
        case(ReassemblyResult::partial):
            return true; // do nothing

        case(ReassemblyResult::complete):
            this->is_rx_ready = !this->ctx.upper->on_rx_ready(ctx.reassembler.get_data());
            return true;

        default: // error
            FORMAT_LOG_BLOCK(this->ctx.logger, levels::warn, "reassembly error: %s", ReassemblyResultSpec::to_string(result));
            return false;
        }
    }

}
