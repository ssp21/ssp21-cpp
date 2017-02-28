
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/ResponderHandshakeStates.h"

using namespace openpal;

namespace ssp21
{
    Responder::Responder(
        const Config& context_config,
        const Session::Config& session_config,
        const openpal::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const Keys& keys) :
        CryptoLayer(
            HandshakeMode::Responder,
            context_config.config,
            session_config,
            logger,
            frame_writer,
            executor,
            keys
        ),
        handshake_state(ResponderHandshake::Idle::get())
    {}

    void Responder::reply_with_handshake_error(HandshakeError err)
    {
        ReplyHandshakeError msg(err);
        const auto res = this->frame_writer->write(msg);
        if (!res.is_error())
        {
            this->lower->start_tx(res.frame);
        }
    }

    HandshakeError Responder::configure_feature_support(const RequestHandshakeBegin& msg)
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

    void Responder::reset_state_on_close()
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
