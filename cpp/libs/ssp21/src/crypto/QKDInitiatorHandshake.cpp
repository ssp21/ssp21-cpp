
#include "QKDInitiatorHandshake.h"

#include "ssp21/util/Exception.h"

#include "log4cpp/LogMacros.h"
#include "ssp21/stack/LogLevels.h"

namespace ssp21 {
QKDInitiatorHandshake::QKDInitiatorHandshake(const log4cpp::Logger& logger, const CryptoSuite& crypto_suite, std::shared_ptr<IKeySource> key_source)
    : logger(logger)
    , crypto_suite(crypto_suite)
    , algorithms(Algorithms::Common::get_or_throw(crypto_suite))
    , key_source(std::move(key_source))
{
}

IInitiatorHandshake::InitResult QKDInitiatorHandshake::initialize_new_handshake()
{
    this->key.reset();

    auto record = this->key_source->consume_key();

    if (!record || !record->key) {
        return InitResult::failure();
    }

    this->key = record->key;

    // write key identifier to the buffer
    auto dest = this->key_id_buffer.as_wseq();
    ser4cpp::BigEndian::write(dest, record->id);

    // The mode data is the key identifier
    return InitResult::success(seq32_t::empty(), this->key_id_buffer.as_seq());
}

void QKDInitiatorHandshake::finalize_request_tx(const seq32_t& request_data, const exe4cpp::steady_time_t& now)
{
    this->time_request_tx = now;
    this->algorithms.handshake.hash({ request_data }, this->handshake_hash);
}

bool QKDInitiatorHandshake::initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& reply_data, const SessionLimits& limits, const exe4cpp::steady_time_t& now, Session& session)
{
    if (!this->key) {
        SIMPLE_LOG_BLOCK(this->logger, levels::error, "shared secret key not initialized");
        return false;
    }

    if (msg.mode_ephemeral.is_not_empty()) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "non-empty mode ephemeral field: %u", msg.mode_ephemeral.length());
        return false;
    }

    if (msg.mode_data.is_not_empty()) {
        FORMAT_LOG_BLOCK(this->logger, levels::warn, "non-empty mode data field: %u", msg.mode_data.length());
        return false;
    }

    // mix the handshake hash
    const auto salt = this->mix_handshake_hash(reply_data);

    // perform session key derivation
    SessionKeys session_keys;

    // perform key derivation
    this->algorithms.handshake.kdf(
        salt,
        { this->key->as_seq() },
        session_keys.tx_key,
        session_keys.rx_key);

    if (now < this->time_request_tx) {
        SIMPLE_LOG_BLOCK(this->logger, levels::error, "clock rollback detected");
        return false;
    }

    // estimate the session initialization time
    const auto elapsed_ms = now - this->time_request_tx;
    const auto session_start_time = now - (elapsed_ms / 2); // estimate

    return session.initialize(
        this->algorithms.session,
        Session::Param(session_start_time, limits.max_nonce_value, std::chrono::milliseconds(limits.max_session_time_ms)),
        session_keys);

    return false;
}

seq32_t QKDInitiatorHandshake::mix_handshake_hash(const seq32_t& input)
{
    // h = hash(h || input)

    this->algorithms.handshake.hash(
        { this->handshake_hash.as_seq(), input },
        this->handshake_hash);

    return this->handshake_hash.as_seq();
}

}
