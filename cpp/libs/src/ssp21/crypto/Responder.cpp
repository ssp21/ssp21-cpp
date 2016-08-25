
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"
#include "ssp21/crypto/LogMessagePrinter.h"
#include "ssp21/msg/ReplyHandshakeError.h"

using namespace openpal;

namespace ssp21
{
	Responder::Responder(const Config& config, openpal::IExecutor& executor, Logger logger, ILowerLayer& lower) :
        config_(config),
		executor_(&executor),
        logger_(logger),
        lower_(&lower),
        tx_buffer_(config.max_tx_message_size)
    {

    }

    void Responder::on_open_impl()
    {

    }

    void Responder::on_close_impl()
    {

    }

    void Responder::on_tx_ready_impl()
    {
        // possibly try to read a buffered message
        lower_->read_message(*this);
    }

    void Responder::on_rx_ready_impl()
    {
        if (!lower_->is_transmitting())
        {
            lower_->read_message(*this);
        }
    }

    template <class MsgType>
    inline void Responder::read_any(const openpal::RSlice& data)
    {
        MsgType msg;
        auto err = msg.read_msg(data);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(logger_, levels::warn, "error reading %s: %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));
            this->handle_parse_error<MsgType>(err);
        }
        else
        {
            this->on_message(data, msg);
        }
    }

    void Responder::process(const Message& message)
    {
        if (message.payload.is_empty())
        {
            SIMPLE_LOG_BLOCK(logger_, levels::warn, "Received zero length message");
            return;
        }

        const auto function = message.payload[0];

        switch (FunctionSpec::from_type(function))
        {
        case(Function::request_handshake_begin) :
            this->read_any<RequestHandshakeBegin>(message.payload);
            break;

        case(Function::request_handshake_auth) :
            this->read_any<RequestHandshakeAuth>(message.payload);
            break;

        case(Function::unconfirmed_session_data) :
            this->read_any<UnconfirmedSessionData>(message.payload);
            break;

        default:
            FORMAT_LOG_BLOCK(logger_, levels::warn, "Received unknown function id: %u", function);
            break;
        }
    }

    void Responder::reply_with_handshake_error(HandshakeError err)
    {
        ReplyHandshakeError msg(err);

        auto dest = this->tx_buffer_.as_wslice();
        auto result = msg.write_msg(dest);

        if (!result.is_error())
        {
            this->lower_->begin_transmit(Addresses(), result.written);
        }
    }

    void Responder::on_message(const RSlice& data, const RequestHandshakeBegin& msg)
    {
        FORMAT_LOG_BLOCK(logger_, levels::rx_crypto_msg, "request handshake begin (length = %u)", data.length());

        if (logger_.is_enabled(levels::rx_crypto_msg_fields))
        {
            LogMessagePrinter printer(logger_, levels::rx_crypto_msg_fields);
            msg.print(printer);
        }
    }

    void Responder::on_message(const RSlice& data, const RequestHandshakeAuth& msg)
    {
        FORMAT_LOG_BLOCK(logger_, levels::rx_crypto_msg, "request handshake auth (length = %u)", data.length());

        if (logger_.is_enabled(levels::rx_crypto_msg_fields))
        {
            LogMessagePrinter printer(logger_, levels::rx_crypto_msg_fields);
            msg.print(printer);
        }
    }

    void Responder::on_message(const RSlice& data, const UnconfirmedSessionData& msg)
    {
        FORMAT_LOG_BLOCK(logger_, levels::rx_crypto_msg, "unconfirmed session data (length = %u)", data.length());

        if (logger_.is_enabled(levels::rx_crypto_msg_fields))
        {
            LogMessagePrinter printer(logger_, levels::rx_crypto_msg_fields);
            msg.print(printer);
        }
    }

}
