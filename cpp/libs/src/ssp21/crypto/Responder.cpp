
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"

#include "ssp21/crypto/LogMessagePrinter.h"
#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/ResponderHandshakeStates.h"

#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/MessageDispatcher.h"

using namespace openpal;

namespace ssp21
{
    Responder::Context::Context(
        const Config& config,
        std::unique_ptr<KeyPair> local_static_key_pair,
        std::unique_ptr<PublicKey> remote_static_public_key,
        const openpal::Logger& logger,
        const std::shared_ptr<openpal::IExecutor>& executor,
        ILowerLayer& lower) :

        config(config),
        local_static_key_pair(std::move(local_static_key_pair)),
        remote_static_public_key(std::move(remote_static_public_key)),
        logger(logger),
        executor(executor),
        handshake(EntityId::Responder),
        session(config.session),
        reassembler(config.max_reassembly_size),
        lower(&lower),
        tx_buffer(config.max_tx_message_size)
    {

    }

    void Responder::Context::reply_with_handshake_error(HandshakeError err)
    {
        ReplyHandshakeError msg(err);

        auto result = this->write_msg(msg);

        if (!result.is_error())
        {
            this->transmit_to_lower(msg, result.written);
        }
    }

    void Responder::Context::log_message(openpal::LogLevel msg_level, openpal::LogLevel field_level, Function func, const IMessage& msg, uint32_t length)
    {
        FORMAT_LOG_BLOCK(this->logger, msg_level, "%s (length = %u)", FunctionSpec::to_string(func), length);

        if (this->logger.is_enabled(field_level))
        {
            LogMessagePrinter printer(this->logger, field_level);
            msg.print(printer);
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
        return handshake.set_algorithms(
                   Algorithms::Config(
                       msg.dh_mode,
                       msg.handshake_hash,
                       msg.nonce_mode,
                       msg.session_mode
                   )
               );
    }

    Responder::Responder(const Config& config,
                         std::unique_ptr<KeyPair> local_static_key_pair,
                         std::unique_ptr<PublicKey> remote_static_public_key,
                         const openpal::Logger& logger,
                         const std::shared_ptr<openpal::IExecutor>& executor,
                         ILowerLayer& lower
                        ) :
        ctx(config, std::move(local_static_key_pair), std::move(remote_static_public_key), logger, executor, lower),
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
        ctx.tx_state.on_tx_complete();

        this->check_receive();
        this->check_transmit();
    }

    void Responder::on_rx_ready_impl()
    {
        this->check_receive();
    }

    void Responder::check_receive()
    {
        /*
            1) the lower layer should have data
            2) the lower layer should be ready to transmit
            3) this layer shouldn't have any un-read data
        */
        const auto can_receive = ctx.lower->get_is_rx_ready() && ctx.lower->get_is_tx_ready() && !this->get_is_rx_ready();
        if (can_receive)
        {
            ctx.lower->receive(*this);
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

            UnconfirmedSessionData msg;
            const auto err = ctx.session.format_message(msg, fir, now, remainder);
            if (err)
            {
                FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "Error formatting session message: %s", err.message().c_str());
                return;
            }

            const auto result = this->ctx.write_msg(msg);

            if (result.is_error())
            {
                FORMAT_LOG_BLOCK(ctx.logger, levels::error, "Error writing session message: %s", FormatErrorSpec::to_string(result.err));
                return;
            }

            ctx.tx_state.begin_transmit(remainder);

            this->ctx.transmit_to_lower(msg, result.written);
        }
    }

    bool Responder::transmit(const openpal::RSlice& data)
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

    bool Responder::receive(IMessageProcessor& processor)
    {
        if (this->is_rx_ready)
        {
            this->is_rx_ready = false;
            processor.process(this->ctx.reassembler.get_data());
            return true;
        }

        return false;
    }

    void Responder::process(const openpal::RSlice& message)
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
        case(Function::unconfirmed_session_data):
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

    bool Responder::on_message(const RequestHandshakeBegin& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
    {
        ctx.log_message(levels::rx_crypto_msg, levels::rx_crypto_msg_fields, msg.function, msg, raw_data.length());
        this->handshake_state = &this->handshake_state->on_message(ctx, raw_data, msg);
        return true;
    }

    bool Responder::on_message(const RequestHandshakeAuth& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
    {
        ctx.log_message(levels::rx_crypto_msg, levels::rx_crypto_msg_fields, msg.function, msg, raw_data.length());
        this->handshake_state = &this->handshake_state->on_message(ctx, raw_data, msg);
        return true;
    }

    bool Responder::on_message(const UnconfirmedSessionData& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
    {
        ctx.log_message(levels::rx_crypto_msg, levels::rx_crypto_msg_fields, Function::unconfirmed_session_data, msg, raw_data.length());

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
            this->is_rx_ready = true;
            this->ctx.upper->on_rx_ready();
            return true;

        default: // error
            FORMAT_LOG_BLOCK(this->ctx.logger, levels::warn, "reassembly error: %s", ReassemblyResultSpec::to_string(result));
            return false;
        }
    }

}
