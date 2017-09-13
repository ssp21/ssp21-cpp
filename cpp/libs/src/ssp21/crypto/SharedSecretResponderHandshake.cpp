
#include "ssp21/crypto/SharedSecretResponderHandshake.h"

#include "ssp21/util/Exception.h"

#include "ssp21/crypto/HandshakeHasher.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"

#include "openpal/logging/LogMacros.h"
#include "openpal/logging/LogLevels.h"

using namespace openpal;

namespace ssp21
{

    SharedSecretResponderHandshake::SharedSecretResponderHandshake(const Logger& logger, const std::shared_ptr<SymmetricKey>& shared_secret) :
        logger(logger),
        shared_secret(shared_secret)
    {
        if (shared_secret->as_seq().length() != consts::crypto::symmetric_ley_length)
        {
            throw Exception("Invalid length for shared secret: ", shared_secret->as_seq().length());
        }
    }

    IResponderHandshake::Result SharedSecretResponderHandshake::process(const RequestHandshakeBegin& msg, const seq32_t& msg_bytes, const Timestamp& now, IFrameWriter& writer, Session& session)
    {
        if (msg.spec.handshake_ephemeral != HandshakeEphemeral::nonce)
        {
            return Result::failure(HandshakeError::unsupported_handshake_ephemeral);
        }

        // verify that the nonce is the correct length
        if (msg.ephemeral_data.length() != consts::crypto::nonce_length)
        {
            return Result::failure(HandshakeError::bad_message_format);
        }

        Algorithms::Session session_algorithms;
        Algorithms::SharedSecretHandshake handshake_algorithms;

        {
            const auto err = session_algorithms.configure(msg.spec.nonce_mode, msg.spec.session_mode);
            if (any(err)) return Result::failure(err);
        }

        {
            const auto err = handshake_algorithms.configure(msg.spec.handshake_kdf, msg.spec.handshake_hash);
            if (any(err)) return Result::failure(err);
        }

        // generate a nonce
        StaticBuffer<uint32_t, consts::crypto::nonce_length> nonce_buffer;
        Crypto::gen_random(nonce_buffer.as_wseq());

        // prepare the response
        const ReplyHandshakeBegin reply(
            nonce_buffer.as_seq(),
            seq32_t::empty()
        );

        const auto result = writer.write(reply);
        if (any(result.err))
        {
            FORMAT_LOG_BLOCK(this->logger, levels::error, "Error writing handshake reply: %s", FormatErrorSpec::to_string(result.err));
            return Result::failure(HandshakeError::internal);
        }

        HandshakeHasher hasher;
        const auto handshake_hash = hasher.compute(handshake_algorithms.hash, msg_bytes, result.written);

        SessionKeys session_keys;

        handshake_algorithms.kdf(
            handshake_hash,
        { this->shared_secret->as_seq() },
        session_keys.rx_key,
        session_keys.tx_key
        );

        session.initialize(
            session_algorithms,
            Session::Param(
                now,
                msg.constraints.max_nonce,
                msg.constraints.max_session_duration
            ),
            session_keys
        );

        return Result::failure(HandshakeError::none);
    }

}

