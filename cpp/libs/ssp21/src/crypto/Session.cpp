
#include "crypto/Session.h"

#include "log4cpp/LogMacros.h"

#include "ssp21/crypto/gen/CryptoError.h"
#include "ssp21/stack/LogLevels.h"

#include <limits>

namespace ssp21 {

Session::Session(std::shared_ptr<IFrameWriter> frame_writer, std::shared_ptr<SessionStatistics> statistics, const SessionConfig& config)
    : frame_writer(std::move(frame_writer))
    , statistics(std::move(statistics))
    , config(config)
    , encrypt_buffer(calc_max_crypto_payload_length(this->frame_writer->get_max_payload_size()))
{
}

constexpr uint32_t Session::calc_max_crypto_payload_length(uint32_t max_link_payload_size)
{
    return (max_link_payload_size > SessionData::min_size_bytes) ? max_link_payload_size - SessionData::min_size_bytes : 0;
}

bool Session::initialize(const Algorithms::Session& algorithms, const Param& parameters, const SessionKeys& keys)
{
    if (!keys.valid())
        return false;

    this->rx_nonce.set(0);
    this->tx_nonce.set(0);
    this->algorithms = algorithms;
    this->parameters = parameters;
    this->keys.copy(keys);

    this->statistics->num_init.increment();

    this->valid = true;

    return true;
}

void Session::reset()
{
    this->valid = false;
    this->keys.zero();
}

seq32_t Session::validate_session_auth(const SessionData& message, const exe4cpp::steady_time_t& now, wseq32_t dest, std::error_code& ec)
{
    return this->validate_session_data_with_nonce_func(message, now, dest, NonceFunctions::equal_to_zero, ec);
}

seq32_t Session::validate_session_data(const SessionData& message, const exe4cpp::steady_time_t& now, wseq32_t dest, std::error_code& ec)
{
    const auto payload = this->validate_session_data_with_nonce_func(message, now, dest, this->algorithms.verify_nonce, ec);

    if (ec) {
        return payload;
    }

    if (payload.is_empty()) {
        this->statistics->num_auth_fail.increment();
        ec = CryptoError::empty_user_data;
    }

    return payload;
}

seq32_t Session::format_session_auth(const exe4cpp::steady_time_t& now, seq32_t& cleartext, std::error_code& ec)
{
    if (!this->tx_nonce.is_zero()) {
        ec = CryptoError::max_nonce_exceeded;
        return seq32_t::empty();
    }

    return this->format_session_data_no_nonce_check(now, cleartext, ec);
}

seq32_t Session::format_session_data(const exe4cpp::steady_time_t& now, seq32_t& cleartext, std::error_code& ec)
{
    if (this->tx_nonce.get() >= this->parameters.max_nonce) {
        ec = CryptoError::max_nonce_exceeded;
        return seq32_t::empty();
    }

    return this->format_session_data_no_nonce_check(now, cleartext, ec);
}

seq32_t Session::validate_session_data_with_nonce_func(const SessionData& message, const exe4cpp::steady_time_t& now, wseq32_t dest, verify_nonce_func_t verify_nonce, std::error_code& ec)
{
    if (!this->valid) {
        this->statistics->num_user_data_without_session.increment();
        ec = CryptoError::no_valid_session;
        return seq32_t::empty();
    }

    const auto payload = this->algorithms.session_mode->read(this->keys.rx_key, message, dest, ec);

    if (ec) {
        this->statistics->num_auth_fail.increment();
        return seq32_t::empty();
    }

    if (now < this->parameters.session_start) {
        ec = CryptoError::clock_rollback;
        return seq32_t::empty();
    }

    const auto current_session_time = now - this->parameters.session_start;

    if (current_session_time > this->parameters.max_session_time) {
        ec = CryptoError::max_session_time_exceeded;
        return seq32_t::empty();
    }

    // the message is authentic, check the TTL
    if (current_session_time > std::chrono::milliseconds(message.metadata.valid_until_ms)) {
        this->statistics->num_ttl_expiration.increment();
        ec = CryptoError::expired_ttl;
        return seq32_t::empty();
    }

    // check the nonce via the configured maximum
    if (message.metadata.nonce > this->parameters.max_nonce) {
        this->statistics->num_nonce_fail.increment();
        ec = CryptoError::max_nonce_exceeded;
        return seq32_t::empty();
    }

    // check the nonce via the verification function
    if (!verify_nonce(this->rx_nonce.get(), message.metadata.nonce)) {
        this->statistics->num_nonce_fail.increment();
        ec = CryptoError::nonce_replay;
        return seq32_t::empty();
    }

    this->rx_nonce.set(message.metadata.nonce.value);

    this->statistics->num_success.increment();

    return payload;
}

seq32_t Session::format_session_data_no_nonce_check(const exe4cpp::steady_time_t& now, seq32_t& clear_text, std::error_code& ec)
{
    if (!this->valid) {
        ec = CryptoError::no_valid_session;
        return seq32_t::empty();
    }

    if (now < this->parameters.session_start) {
        ec = CryptoError::clock_rollback;
        return seq32_t::empty();
    }

    const auto session_time = now - this->parameters.session_start;

    if (session_time > this->parameters.max_session_time) {
        ec = CryptoError::max_session_time_exceeded;
        return seq32_t::empty();
    }

    const auto remainder = this->parameters.max_session_time - session_time;
    if (remainder < std::chrono::milliseconds(config.ttl_pad_ms)) {
        ec = CryptoError::max_session_time_exceeded;
        return seq32_t::empty();
    }

    // the metadata we're encoding
    const AuthMetadata metadata(
        this->tx_nonce.get(),
        static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(session_time + std::chrono::milliseconds(config.ttl_pad_ms)).count()));

    MACOutput mac;

    const auto message = this->algorithms.session_mode->write(this->keys.tx_key, metadata, clear_text, this->encrypt_buffer.as_wslice(), mac, ec);
    if (ec) {
        return seq32_t::empty();
    }

    // now serialize the message
    const auto result = this->frame_writer->write(message);

    if (result.is_error()) {
        ec = result.err;
        return seq32_t::empty();
    }

    // everything succeeded, so increment the nonce
    this->tx_nonce.increment();

    return result.frame;
}

}
