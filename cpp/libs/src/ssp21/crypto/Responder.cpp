
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
        std::unique_ptr<KeyPair> local_static_key_pair,
        std::unique_ptr<PublicKey> remote_static_public_key) :
        CryptoLayer(
            EntityType::Responder,
            context_config,
            session_config,
            logger,
            frame_writer,
            executor,
            std::move(local_static_key_pair),
            std::move(remote_static_public_key)
        ),
        handshake_state(&HandshakeIdle::get())
    {}

    void Responder::reply_with_handshake_error(HandshakeError err)
    {
        ReplyHandshakeError msg(err);
        const auto res = this->frame_writer->write(msg);
        if (!res.is_error())
        {
            this->lower->transmit(res.frame);
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

    void Responder::on_close_impl()
    {
        this->handshake_state = &HandshakeIdle::get();

        this->session.reset();
        this->reassembler.reset();
        this->upper->on_close();
        this->tx_state.reset();

        this->reset_lower_layer();
    }

    void Responder::on_tx_ready_impl()
    {
        if (this->tx_state.on_tx_complete())
        {
            // ready to transmit more data
            this->is_tx_ready = true;
            this->upper->on_tx_ready();
        }

        this->check_receive();
        this->check_transmit();
    }    

    void Responder::check_receive()
    {
        if (this->can_receive())
        {
            this->lower->receive();
        }
    }

    void Responder::check_transmit()
    {
        /**
        * 1) The session must be able to transmit
        * 2) Lower-layer must be ready to transmit
        * 3) transmission state must have some data to send
        */
        if (this->session.is_valid() && this->lower->get_is_tx_ready() && this->tx_state.is_ready_tx())
        {
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
                this->upper->on_close();

                return;
            }

            this->tx_state.begin_transmit(remainder);

            this->lower->transmit(data);
        }
    }

    bool Responder::transmit(const seq32_t& data)
    {
        if (!this->get_is_open())
        {
            return false;
        }

        if (!this->upper->get_is_open())
        {
            return false;
        }

        // already transmitting on behalf on the upper layer
        if (!tx_state.initialize(data))
        {
            return false;
        }

        this->check_transmit();

        return true;
    }

    void Responder::receive()
    {
        if (this->is_rx_ready && this->upper->on_rx_ready(this->reassembler.get_data()))
        {
            this->is_rx_ready = false;
        }
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

    bool Responder::on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = &this->handshake_state->on_message(*this, raw_data, msg);
        return true;
    }

    bool Responder::on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        this->handshake_state = &this->handshake_state->on_message(*this, raw_data, msg);
        return true;
    }

    bool Responder::on_message(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        std::error_code ec;
        const auto payload = this->session.validate_message(msg, now, ec);

        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "validation error: %s", ec.message().c_str());
            return false;
        }

        // process the message using the reassembler
        const auto result = this->reassembler.process(msg.metadata.flags.fir, msg.metadata.flags.fin, msg.metadata.nonce, payload);

        switch (result)
        {
        case(ReassemblyResult::partial):
            return true; // do nothing

        case(ReassemblyResult::complete):
            this->is_rx_ready = !this->upper->on_rx_ready(this->reassembler.get_data());
            return true;

        default: // error
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "reassembly error: %s", ReassemblyResultSpec::to_string(result));
            return false;
        }
    }

}
