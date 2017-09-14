
#include "SharedSecretInitiatorHandshake.h"

#include "ssp21/util/Exception.h"

#include "openpal/logging/LogMacros.h"
#include "ssp21/stack/LogLevels.h"

namespace ssp21
{
    SharedSecretInitiatorHandshake::SharedSecretInitiatorHandshake(const openpal::Logger& logger, const CryptoSuite& crypto_suite, const std::shared_ptr<const SymmetricKey>& shared_secret) :
        logger(logger),
        shared_secret(shared_secret),
        crypto_suite(crypto_suite),
        algorithms(algorithms)
    {
        if (crypto_suite.handshake_ephemeral != HandshakeEphemeral::nonce)
        {
            throw new Exception("handshake_ephemeral must be nonce");
        }
    }

    seq32_t SharedSecretInitiatorHandshake::generate_ephemeral_data()
    {
        Crypto::gen_random(this->nonce_buffer.as_wseq());
        return this->nonce_buffer.as_seq();
    }

    void SharedSecretInitiatorHandshake::begin_request_transmit(const seq32_t& data, const openpal::Timestamp& now)
    {
        this->time_request_tx = now;
        this->algorithms.handshake.hash({ data }, this->handshake_hash);
    }

    bool SharedSecretInitiatorHandshake::initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const InitiatorConfig::Params& params, const openpal::Timestamp& now, Session& session)
    {
        if (msg.ephemeral_data.length() != consts::crypto::nonce_length)
        {
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "bad nonce length: %u", msg.ephemeral_data.length());
            return false;
        }

        if (msg.handshake_data.is_not_empty())
        {
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "non-empty handshake data: %u", msg.handshake_data.length());
            return false;
        }

        // mix the handshake hash
        const auto salt = this->mix_handshake_hash(msg_bytes);

        // perform session key derivation
        SessionKeys session_keys;

        // perform key derivation
        this->algorithms.handshake.kdf(
            salt,
        { this->shared_secret->as_seq() },
        session_keys.tx_key,
        session_keys.rx_key
        );

        if (now < this->time_request_tx)
        {
            SIMPLE_LOG_BLOCK(this->logger, levels::error, "clock rollback detected");
            return false;
        }

        // estimate the session initialization time
        const auto elapsed_ms = now.milliseconds - this->time_request_tx.milliseconds;
        const auto session_start_time = openpal::Timestamp(now.milliseconds - (elapsed_ms / 2)); // estimate

        return session.initialize(
                   this->algorithms.session,
                   Session::Param(session_start_time, params.max_nonce_value, params.max_session_time_ms),
                   session_keys
               );

        return false;
    }

    seq32_t SharedSecretInitiatorHandshake::mix_handshake_hash(const seq32_t& input)
    {
        // h = hash(h || input)

        this->algorithms.handshake.hash(
        { this->handshake_hash.as_seq(), input },
        this->handshake_hash
        );

        return this->handshake_hash.as_seq();
    }

}


