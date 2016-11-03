
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"

#include "ssp21/crypto/LogMessagePrinter.h"
#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/ResponderHandshakeStates.h"

#include "ssp21/crypto/gen/ReplyHandshakeError.h"

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
                       msg.hash_mode,
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

    void Responder::on_open_impl()
    {

    }

    void Responder::on_close_impl()
    {
        handshake_state = &HandshakeIdle::get();
    }

    void Responder::on_tx_ready_impl()
    {
        // only read a message we don't have data ready
        if (ctx.lower->is_rx_ready() && this->can_receive())
        {
            ctx.lower->receive(*this);
        }
    }

    void Responder::on_rx_ready_impl()
    {
        // only read a message if the lower layer
        // can transmit a response
        if (this->can_receive())
        {
            ctx.lower->receive(*this);
        }
    }

    bool Responder::transmit(const Message& message)
    {
        // TODO
        return false;
    }

    bool Responder::receive(IMessageProcessor& processor)
    {
        // TODO
        return false;
    }

    template <class MsgType>
    inline void Responder::handle_handshake_message(const openpal::RSlice& data)
    {
        MsgType msg;
        auto err = msg.read(data);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "error reading %s: %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));
            ctx.reply_with_handshake_error(HandshakeError::bad_message_format);
        }
        else
        {
            ctx.log_message(levels::rx_crypto_msg, levels::rx_crypto_msg_fields, MsgType::function, msg, data.length());

            this->handshake_state = &this->handshake_state->on_message(ctx, data, msg);
        }
    }

    void Responder::handle_session_message(const openpal::RSlice& data)
    {
        UnconfirmedSessionData msg;
        auto err = msg.read(data);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "error reading session message: %s", ParseErrorSpec::to_string(err));
            return;
        }

        ctx.log_message(levels::rx_crypto_msg, levels::rx_crypto_msg_fields, Function::unconfirmed_session_data, msg, data.length());

        // TODO - authenticate and process the message
    }

    void Responder::process(const Message& message)
    {
        if (message.addresses.destination != ctx.config.local_address)
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::info, "unknown destination address: %u", message.addresses.destination);
            return;
        }

        if (message.addresses.source != ctx.config.remote_address)
        {
            FORMAT_LOG_BLOCK(ctx.logger, levels::info, "unknown source address: %u", message.addresses.source);
            return;
        }

        if (message.payload.is_empty())
        {
            SIMPLE_LOG_BLOCK(ctx.logger, levels::warn, "Received zero length message");
            return;
        }

        const auto function = message.payload[0];

        switch (FunctionSpec::from_type(function))
        {
        case(Function::request_handshake_begin) :
            this->handle_handshake_message<RequestHandshakeBegin>(message.payload);
            break;

        case(Function::request_handshake_auth) :
            this->handle_handshake_message<RequestHandshakeAuth>(message.payload);
            break;

        case(Function::unconfirmed_session_data) :
            this->handle_session_message(message.payload);
            break;

        default:
            FORMAT_LOG_BLOCK(ctx.logger, levels::warn, "Received unknown function id: %u", function);
            break;
        }
    }

}
