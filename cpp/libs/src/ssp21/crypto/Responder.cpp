
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/stack/LogLevels.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/ResponderHandshakeStates.h"

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
			static_keys,
            certificate_handler
        ),
        handshake_state(ResponderHandshake::Idle::get())
    {}

    void Responder::reply_with_handshake_error(HandshakeError err)
    {
        ReplyHandshakeError msg(err);
        const auto res = this->frame_writer->write(msg);
        if (!res.is_error())
        {
            this->lower->start_tx_from_upper(res.frame);
        }
    }

    HandshakeError Responder::verify_handshake_begin(const RequestHandshakeBegin& msg, seq32_t& public_key_out)
    {
        // TODO - carefully consider the order of operations here and add it to the spec.

        if (msg.version != consts::crypto::protocol_version)
        {
            return HandshakeError::unsupported_version;
        }

        // verify that the public key length matches the DH mode
        if (msg.ephemeral_public_key.length() != consts::crypto::x25519_key_length)
        {
            return HandshakeError::bad_message_format;
        }

        const auto err = this->certificate_handler->validate(msg.certificate_mode, msg.certificate_data, public_key_out);
        if (any(err))
        {
            return err;
        }

        // last thing we should do is try configure the requested algorithms
        return handshake.set_algorithms(msg.spec);
    }

    void Responder::reset_state_on_close_from_lower()
    {
        this->handshake_state = ResponderHandshake::Idle::get();
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
            this->reply_with_handshake_error(HandshakeError::bad_message_format);
            break;
        default:
            break;
        }
    }

    void Responder::on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

    void Responder::on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = this->handshake_state->on_message(*this, msg, raw_data, now);
    }

}
