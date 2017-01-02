
#include "ssp21/crypto/MessageDispatcher.h"

#include "openpal/logging/LogMacros.h"
#include "ssp21/LogLevels.h"

#include "ssp21/crypto/LogMessagePrinter.h"

namespace ssp21
{
    template <class MsgType>
    bool MessageDispatcher::handle_message(openpal::Logger& logger, const seq32_t& message, const openpal::Timestamp& now, IMessageHandler& handler)
    {
        MsgType msg;
        auto err = msg.read(message);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(logger, levels::warn, "Error parsing message (%s): %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));

            handle_parse_error(logger, MsgType::function, err, handler);
            return false;
        }
        else
        {
            FORMAT_LOG_BLOCK(logger, levels::rx_crypto_msg, "%s", FunctionSpec::to_string(MsgType::function));

            if (logger.is_enabled(levels::rx_crypto_msg_fields))
            {
                LogMessagePrinter printer(logger, levels::rx_crypto_msg_fields);
                msg.print(printer);
            }

            handler.on_message(msg, message, now);

            return true;
        }
    }

    bool MessageDispatcher::Dispatch(openpal::Logger& logger, const seq32_t& message, const openpal::Timestamp& now, IMessageHandler& handler)
    {
        if (message.is_empty())
        {
            SIMPLE_LOG_BLOCK(logger, levels::warn, "Received zero length message");
            return false;
        }

        const auto raw_function = message[0];
        const auto function = FunctionSpec::from_type(raw_function);

        if (!handler.supports(function))
        {
            FORMAT_LOG_BLOCK(logger, levels::warn, "Received unsupported function: %s(%u)", FunctionSpec::to_string(function), raw_function);
            return false;
        }

        switch (function)
        {
        case(Function::request_handshake_begin):
            return handle_message<RequestHandshakeBegin>(logger, message, now, handler);
        case(Function::request_handshake_auth):
            return handle_message<RequestHandshakeAuth>(logger, message, now, handler);
        case(Function::reply_handshake_begin):
            return handle_message<ReplyHandshakeBegin>(logger, message, now, handler);
        case(Function::reply_handshake_auth):
            return handle_message<ReplyHandshakeAuth>(logger, message, now, handler);
        case(Function::reply_handshake_error):
            return handle_message<ReplyHandshakeError>(logger, message, now, handler);
        case(Function::session_data):
            return handle_message<SessionData>(logger, message, now, handler);
        default:
            return false;
        }
    }

    void MessageDispatcher::handle_parse_error(openpal::Logger& logger, Function function, ParseError error, IMessageHandler& handler)
    {
        FORMAT_LOG_BLOCK(logger, levels::warn, "Error parsing function %s: %s", FunctionSpec::to_string(function), ParseErrorSpec::to_string(error));
        handler.on_parse_error(function, error);
    }
}

