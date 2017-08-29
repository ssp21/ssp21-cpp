
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/stack/LogLevels.h"

using namespace openpal;

namespace ssp21
{
    Responder::Responder(
        const ResponderConfig& config,
        const openpal::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& certificate_handler) :
        CryptoLayer(
            HandshakeMode::Responder,
            config.config,
            config.session,
            logger,
            frame_writer,
            executor,            
            certificate_handler
        ),
		handshake(logger, static_keys, certificate_handler)
    {}

    void Responder::reply_with_handshake_error(HandshakeError err)
    {
        const ReplyHandshakeError msg(err);
        const auto res = this->frame_writer->write(msg);
        if (!res.is_error())
        {
            this->lower->start_tx_from_upper(res.frame);
        }
    }    

    void Responder::reset_state_on_close_from_lower()
    {
        this->sessions.reset_both();
    }

    bool Responder::supports(Function function) const
    {
        switch (function)
        {
        case(Function::request_handshake_begin):
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
            this->reply_with_handshake_error(HandshakeError::bad_message_format);
            break;
        default:
            break;
        }
    }

    void Responder::on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_msg, const openpal::Timestamp& now)
    {
		if (msg.version != consts::crypto::protocol_version)
		{
			FORMAT_LOG_BLOCK(this->logger, levels::warn, "Handshake request with unsupported version: %u", msg.version.value);
			this->reply_with_handshake_error(HandshakeError::unsupported_version);
			return;
		}

		const auto result = this->handshake.process(msg, raw_msg, now, *this->frame_writer, *this->sessions.pending);

		if (any(result.error))
		{
			FORMAT_LOG_BLOCK(this->logger, levels::warn, "Error processing handshake request: %s", HandshakeErrorSpec::to_string(result.error));
			this->reply_with_handshake_error(result.error);
			return;
		}
		else
		{
			// start writing the reply
			this->lower->start_tx_from_upper(result.reply_data);
		}
              
    }

    void Responder::on_auth_session(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {

    }

}
