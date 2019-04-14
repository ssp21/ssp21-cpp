
#include "crypto/SharedSecretResponderHandshake.h"

#include "ssp21/util/Exception.h"

#include "crypto/HandshakeHasher.h"
#include "crypto/AlgorithmSet.h"
#include "crypto/gen/ReplyHandshakeBegin.h"

#include "log4cpp/LogMacros.h"
#include "ssp21/stack/LogLevels.h"

namespace ssp21
{

    SharedSecretResponderHandshake::SharedSecretResponderHandshake(const log4cpp::Logger& logger, const std::shared_ptr<const SymmetricKey>& key) :
        logger(logger),
        key(key)
    {

    }

    IResponderHandshake::Result SharedSecretResponderHandshake::process(const RequestHandshakeBegin& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now, IFrameWriter& writer, Session& session)
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

        shared_secret_algorithms_t algorithms;

        {
            const auto err = algorithms.configure(msg.spec);
            if (any(err)) return Result::failure(err);
        }

        // generate a nonce
        ser4cpp::StaticBuffer<uint32_t, consts::crypto::nonce_length> nonce_buffer;
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
            return Result::failure(HandshakeError::unknown);
        }

        HandshakeHasher hasher;
        const auto handshake_hash = hasher.compute(algorithms.handshake.hash, msg_bytes, result.written);

        SessionKeys session_keys;

        algorithms.handshake.kdf(
            handshake_hash,
        { this->key->as_seq(), msg.ephemeral_data, nonce_buffer.as_seq() },
        session_keys.rx_key,
        session_keys.tx_key
        );

        session.initialize(
            algorithms.session,
            Session::Param(
                now,
                msg.constraints.max_nonce,
                std::chrono::milliseconds(msg.constraints.max_session_duration)
            ),
            session_keys
        );

        return Result::success(result.frame);
    }

}

