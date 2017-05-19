
#include "ssp21/crypto/CryptoLayer.h"

#include "ssp21/crypto/LogMessagePrinter.h"
#include "ssp21/stack/LogLevels.h"

#include "openpal/logging/LogMacros.h"

namespace ssp21
{

    CryptoLayer::CryptoLayer(
        HandshakeMode type,
        const CryptoLayerConfig& context_config,
        const SessionConfig& session_config,
        const openpal::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const Keys& keys
    ) :
        logger(logger),
        frame_writer(frame_writer),
        executor(executor),
        keys(keys),
        handshake(type),
        session(frame_writer, session_config),
        reassembler(context_config.max_reassembly_size)
    {}

    void CryptoLayer::discard_rx_data()
    {
        this->reassembler.reset();
    }

    bool CryptoLayer::start_tx_from_upper(const seq32_t& data)
    {
        if (this->is_tx_ready())
        {
            this->tx_state.initialize(data);
            this->check_transmit();
            return true;
        }
        else
        {
            return false;
        }
    }

    seq32_t CryptoLayer::start_rx_from_upper_impl()
    {
        if (this->reassembler.has_data()) return this->reassembler.get_data();
        else
        {
            this->try_read_from_lower();
            return seq32_t::empty();
        }
    }

    bool CryptoLayer::is_tx_ready() const
    {
        /*
           1) This layer must be open
           2) We must have already opened the upper layer
           3) We shouldn't already be transmitting on behalf of the upper layer
        */

        return this->is_open() && this->upper->is_open() && this->tx_state.is_idle();
    }

    template <class MsgType>
    bool CryptoLayer::handle_message(const seq32_t& message, const openpal::Timestamp& now)
    {
        MsgType msg;
        auto err = msg.read(message);
        if (any(err))
        {
            FORMAT_LOG_BLOCK(logger, levels::warn, "Error parsing message (%s): %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));
            this->on_parse_error(MsgType::function, err);
            return false;
        }
        else
        {
            FORMAT_LOG_BLOCK(this->logger, levels::rx_crypto_msg, "%s (length = %u)", FunctionSpec::to_string(MsgType::function), message.length());

            if (this->logger.is_enabled(levels::rx_crypto_msg_fields))
            {
                LogMessagePrinter printer(this->logger, levels::rx_crypto_msg_fields);
                msg.print(printer);
            }

            this->on_message(msg, message, now);

            return true;
        }
    }

    void CryptoLayer::on_lower_close_impl()
    {
        // let the super class reset
        this->reset_state_on_close_from_lower();

        this->session.reset();
        this->reassembler.reset();
        this->upper->on_lower_close();
        this->tx_state.reset();
        this->reset_this_lower_layer();
    }

    void CryptoLayer::on_lower_rx_ready_impl()
    {
        this->try_read_from_lower();
    }

    void CryptoLayer::on_lower_tx_ready_impl()
    {
        // TODO - evaluate the appropriate priority of these checks

        const bool user_data_tx_complete = this->tx_state.on_tx_complete();
        const bool upper_rx_processing = this->is_upper_processing_rx();

        FORMAT_LOG_BLOCK(logger, levels::debug, "on tx ready, user tx complete = %d rx processing = %d", user_data_tx_complete, upper_rx_processing);

        this->on_pre_tx_ready();

        if (user_data_tx_complete)
        {
            // ready to transmit more data
            this->upper->on_lower_tx_ready();
        }

        this->try_read_from_lower();

        this->check_transmit();
    }

    void CryptoLayer::try_read_from_lower()
    {
        while (this->try_read_one_from_lower());
    }

    bool CryptoLayer::try_read_one_from_lower()
    {
        /**
        * We can only read data if
        *
        * 1) We can immediately transmit a reply if required
        * 2) There isn't unread session data buffered for the upper layer
        *
        */
        if (!this->lower->is_tx_ready() || this->reassembler.has_data()) return false;

        const seq32_t message = this->lower->start_rx_from_upper();
        if (message.is_empty()) return false;

        this->process(message);
        return true;
    }

    void CryptoLayer::process(const seq32_t& message)
    {
        const auto now = this->executor->get_time();

        if (message.is_empty())
        {
            SIMPLE_LOG_BLOCK(logger, levels::warn, "Received zero length message");
            return;
        }

        const auto raw_function = message[0];
        const auto function = FunctionSpec::from_type(raw_function);

        if (!this->supports(function))
        {
            FORMAT_LOG_BLOCK(logger, levels::warn, "Received unsupported function: %s(%u)", FunctionSpec::to_string(function), raw_function);
            return;
        }

        switch (function)
        {
        case(Function::request_handshake_begin):
            handle_message<RequestHandshakeBegin>(message, now);
            break;
        case(Function::request_handshake_auth):
            handle_message<RequestHandshakeAuth>(message, now);
            break;
        case(Function::reply_handshake_begin):
            handle_message<ReplyHandshakeBegin>(message, now);
            break;
        case(Function::reply_handshake_auth):
            handle_message<ReplyHandshakeAuth>(message, now);
            break;
        case(Function::reply_handshake_error):
            handle_message<ReplyHandshakeError>(message, now);
            break;
        case(Function::session_data):
            handle_message<SessionData>(message, now);
            break;
        default:
            break;
        }
    }

    void CryptoLayer::check_transmit()
    {
        if (!this->can_transmit_session_data())
        {
            return;
        }

        auto remainder = this->tx_state.get_remainder();
        const auto fir = this->tx_state.get_fir();
        const auto now = this->executor->get_time();

        std::error_code err;
        const auto data = this->session.format_session_message(fir, now, remainder, err);
        if (err)
        {
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "Error formatting session message: %s", err.message().c_str());

            // if any error occurs with transmission, we reset the session and notify the upper layer
            this->session.reset();
            this->upper->on_lower_close();

            return;
        }

        this->tx_state.begin_transmit(remainder);

        this->lower->start_tx_from_upper(data);

        this->on_session_nonce_change(this->session.get_rx_nonce(), this->session.get_tx_nonce());
    }

    void CryptoLayer::on_message(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        std::error_code ec;
        const auto payload = this->session.validate_message(msg, now, ec);

        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "validation error: %s", ec.message().c_str());
            return;
        }

        this->on_session_nonce_change(this->session.get_rx_nonce(), this->session.get_tx_nonce());

        // process the message using the reassembler
        const auto result = this->reassembler.process(msg.metadata.flags.fir, msg.metadata.flags.fin, msg.metadata.nonce, payload);

        switch (result)
        {
        case(ReassemblyResult::partial):
            break; // do nothing

        case(ReassemblyResult::complete):
            this->upper->on_lower_rx_ready();
            break;

        default: // error
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "reassembly error: %s", ReassemblyResultSpec::to_string(result));
            break;
        }
    }


}

