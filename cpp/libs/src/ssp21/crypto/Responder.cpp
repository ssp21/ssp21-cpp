
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/stack/LogLevels.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/HandshakeHasher.h"
#include "ssp21/crypto/TripleDH.h"

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
		static_keys(static_keys)
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

    HandshakeError Responder::verify_handshake_begin(const RequestHandshakeBegin& msg, Algorithms& algorithms, seq32_t& public_key_out)
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

        {
            const auto err = this->certificate_handler->validate(msg.certificate_mode, msg.certificate_data, public_key_out);
            if (any(err))
            {
                return err;
            }
        }

        return algorithms.configure(msg.spec);
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

    void Responder::on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {
        Algorithms algorithms;
        seq32_t remote_public_dh_key;

        {
            const auto err = this->verify_handshake_begin(msg, algorithms, remote_public_dh_key);
            if (any(err))
            {
                this->reply_with_handshake_error(err);
                return;
            }
        }

        // --- now, with the algorithms and public key determined, start formatting a response ---

        // generate an ephemeral key pair
        KeyPair ephemeralKeys;
        algorithms.handshake.gen_keypair(ephemeralKeys);

        // prepare the response
        const ReplyHandshakeBegin reply(
            ephemeralKeys.public_key.as_seq(),
            this->certificate_handler->certificate_data()
        );

        const auto result = this->frame_writer->write(reply);
        if (any(result.err))
        {
            FORMAT_LOG_BLOCK(this->logger, levels::error, "Error writing handshake reply: %s", FormatErrorSpec::to_string(result.err));
            return;
        }

        // compute the handshake hash
        HandshakeHasher hasher;
        const auto handshake_hash = hasher.compute(algorithms.handshake.hash, raw_data, result.written);

        // compute the input_key_material
        TripleDH triple_dh;
        std::error_code ec;
        const auto input_key_material = triple_dh.compute(
                                            algorithms.handshake.dh,
                                            this->static_keys,
                                            ephemeralKeys,
                                            remote_public_dh_key,
                                            msg.ephemeral_public_key,
                                            ec
                                        );

        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, levels::error, "Error calculating input key material: %s", ec.message().c_str());
            return;
        }

        // temporary copy of session keys
        // TODO directly dervice into Session's copy?
        SessionKeys session_keys;

        algorithms.handshake.kdf(
            handshake_hash,
            input_key_material,
            session_keys.rx_key,
            session_keys.tx_key
        );

        this->sessions.pending->initialize(
            algorithms.session,
            Session::Param(
                now,
                msg.constraints.max_nonce,
                msg.constraints.max_session_duration
            ),
            session_keys
        );

        // start writing the reply
        this->lower->start_tx_from_upper(result.frame);
    }

    void Responder::on_auth_session(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now)
    {

    }

}
