
#include "crypto/Responder.h"

#include "log4cpp/LogMacros.h"

#include "crypto/ProtocolVersion.h"
#include "ssp21/stack/LogLevels.h"

namespace ssp21 {
Responder::Responder(
    const ResponderConfig& config,
    const log4cpp::Logger& logger,
    const std::shared_ptr<IFrameWriter>& frame_writer,
    const std::shared_ptr<exe4cpp::IExecutor>& executor,
    const std::shared_ptr<IResponderHandshake>& handshake)
    : CryptoLayer(
        config.config,
        config.session,
        logger,
        frame_writer,
        executor)
    , handshake(handshake)
{
}

void Responder::reply_with_handshake_error(HandshakeError err)
{
    const ReplyHandshakeError msg(
        version::get(),
        err);
    const auto res = this->frame_writer->write(msg);
    if (!res.is_error()) {
        this->lower->start_tx_from_upper(res.frame);
    }
}

void Responder::reset_state_on_close_from_lower()
{
    this->sessions.reset_both();
}

bool Responder::supports(Function function) const
{
    switch (function) {
    case (Function::request_handshake_begin):
    case (Function::session_data):
        return true;
    default:
        return false;
    }
}

void Responder::on_parse_error(Function function, ParseError error)
{
    switch (function) {
    case (Function::request_handshake_begin):
        this->reply_with_handshake_error(HandshakeError::bad_message_format);
        break;
    default:
        break;
    }
}

void Responder::on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_msg, const exe4cpp::steady_time_t& now)
{
    if (msg.version.major_version.value != consts::crypto::protocol_major_version) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "Handshake request with unsupported major version: %u", msg.version.major_version.value);
        this->reply_with_handshake_error(HandshakeError::unsupported_version);
        return;
    }

    const auto result = this->handshake->process(msg, raw_msg, now, *this->frame_writer, *this->sessions.pending);

    if (any(result.error)) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "Error processing handshake request: %s", HandshakeErrorSpec::to_string(result.error));
        this->reply_with_handshake_error(result.error);
    } else {
        // start writing the reply
        this->lower->start_tx_from_upper(result.reply_data);
    }
}

void Responder::on_auth_session(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
{
    if (!this->sessions.pending->is_valid()) {
        this->reply_with_handshake_error(HandshakeError::no_prior_handshake_begin);
        return;
    }

    std::error_code ec;
    const auto payload = this->sessions.pending->validate_session_auth(msg, now, payload_buffer.as_wslice(), ec);

    if (ec) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "Error processing session auth request: %s", ec.message().c_str());
        return;
    }

    this->sessions.activate_pending();

    if (!this->transmit_session_auth(*this->sessions.active)) {
        return;
    }

    this->upper->on_lower_open();

    // notify the upper layer there is data ready
    if (payload.is_not_empty()) {
        this->payload_data = payload;
        this->upper->on_lower_rx_ready();
    }
}

}
