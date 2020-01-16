
#include "crypto/CryptoLayer.h"

#include "crypto/LogMessagePrinter.h"
#include "ssp21/stack/LogLevels.h"

#include "log4cpp/LogMacros.h"

namespace ssp21 {

CryptoLayer::CryptoLayer(
    const CryptoLayerConfig& context_config,
    const SessionConfig& session_config,
    const log4cpp::Logger& logger,
    const std::shared_ptr<IFrameWriter>& frame_writer,
    const std::shared_ptr<exe4cpp::IExecutor>& executor)
    : logger(logger)
    , frame_writer(frame_writer)
    , executor(executor)
    , statistics(std::make_shared<SessionStatistics>())
    , sessions(frame_writer, statistics, session_config)
    , payload_buffer(context_config.max_payload_size)
{
}

void CryptoLayer::discard_rx_data()
{
    this->payload_data.make_empty();
}

bool CryptoLayer::start_tx_from_upper(const seq32_t& data)
{
    if (this->is_tx_ready()) {
        this->tx_state.initialize(data);
        this->check_transmit();
        return true;
    } else {
        return false;
    }
}

seq32_t CryptoLayer::start_rx_from_upper_impl()
{
    if (this->payload_data.is_empty()) {
        this->try_read_from_lower();
        return seq32_t::empty();
    } else {
        return this->payload_data;
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
bool CryptoLayer::handle_message(const seq32_t& message, const exe4cpp::steady_time_t& now)
{
    MsgType msg;
    auto err = msg.read(message);
    if (any(err)) {
        FORMAT_LOG_BLOCK(logger, levels::warn, "Error parsing message (%s): %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));
        this->on_parse_error(MsgType::function, err);
        return false;
    } else {
        FORMAT_LOG_BLOCK(this->logger, levels::rx_crypto_msg, "%s (length = %u)", FunctionSpec::to_string(MsgType::function), message.length());

        if (this->logger.is_enabled(levels::rx_crypto_msg_fields)) {
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

    this->sessions.reset_both();
    this->payload_data.make_empty();
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

    if (user_data_tx_complete) {
        // ready to transmit more data
        this->upper->on_lower_tx_ready();
    }

    this->try_read_from_lower();

    this->check_transmit();
}

void CryptoLayer::try_read_from_lower()
{
    while (this->try_read_one_from_lower())
        ;
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

    if (!this->lower->is_tx_ready() || this->payload_data.is_not_empty())
        return false;

    const seq32_t message = this->lower->start_rx_from_upper();
    if (message.is_empty())
        return false;

    this->process(message);
    return true;
}

void CryptoLayer::process(const seq32_t& message)
{
    const auto now = this->executor->get_time();

    if (message.is_empty()) {
        SIMPLE_LOG_BLOCK(logger, levels::warn, "Received zero length message");
        return;
    }

    const auto raw_function = message[0];
    const auto function = FunctionSpec::from_type(raw_function);

    if (!this->supports(function)) {
        FORMAT_LOG_BLOCK(logger, levels::warn, "Received unsupported function: %s(%u)", FunctionSpec::to_string(function), raw_function);
        return;
    }

    switch (function) {
    case (Function::request_handshake_begin):
        handle_message<RequestHandshakeBegin>(message, now);
        break;
    case (Function::reply_handshake_begin):
        handle_message<ReplyHandshakeBegin>(message, now);
        break;
    case (Function::reply_handshake_error):
        handle_message<ReplyHandshakeError>(message, now);
        break;
    case (Function::session_data):
        handle_message<SessionData>(message, now);
        break;
    default:
        break;
    }
}

void CryptoLayer::check_transmit()
{
    if (!this->can_transmit_session_data()) {
        return;
    }

    auto remainder = this->tx_state.get_remainder();
    const auto now = this->executor->get_time();

    std::error_code ec;
    const auto frame = this->sessions.active->format_session_data(now, remainder, ec);
    if (ec) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "Error formatting session message: %s", ec.message().c_str());

        // if any error occurs with transmission, we reset the session and notify the upper layer
        this->sessions.active.reset();
        this->upper->on_lower_close();

        return;
    }

    this->tx_state.begin_transmit(remainder);

    this->lower->start_tx_from_upper(frame);

    this->on_session_nonce_change(this->sessions.active->get_rx_nonce(), this->sessions.active->get_tx_nonce());
}

bool CryptoLayer::transmit_session_auth(Session& session)
{
    auto remainder = this->tx_state.get_remainder();
    const bool has_payload = remainder.is_not_empty();

    std::error_code ec;
    const auto frame = session.format_session_auth(this->executor->get_time(), remainder, ec);
    if (ec) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "Error formatting session auth message: %s", ec.message().c_str());

        // TODO any other actions?

        return false;
    }

    if (has_payload) {
        this->tx_state.begin_transmit(remainder);
    }

    return this->lower->start_tx_from_upper(frame);
}

void CryptoLayer::on_message(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
{
    // differentiate
    if (msg.metadata.nonce == 0) {
        this->on_auth_session(msg, raw_data, now);
    } else {
        this->on_session_data(msg, raw_data, now);
    }
}

void CryptoLayer::on_session_data(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
{
    std::error_code ec;
    const auto payload = this->sessions.active->validate_session_data(msg, now, this->payload_buffer.as_wslice(), ec);

    if (ec) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "error reading session data: %s", ec.message().c_str());
        return;
    }

    this->payload_data = payload;

    this->on_session_nonce_change(this->sessions.active->get_rx_nonce(), this->sessions.active->get_tx_nonce());

    this->upper->on_lower_rx_ready();
}

}
