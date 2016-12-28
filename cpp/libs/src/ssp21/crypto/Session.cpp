
#include "ssp21/crypto/Session.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include <limits>

using namespace openpal;

namespace ssp21
{

    Session::Session(const Config& config) :
        config(config),
        max_crypto_payload_length(calc_max_crypto_payload_length(config.max_link_payload_length)),
        rx_auth_buffer(max_crypto_payload_length),
        tx_encrypt_user_data_buffer(max_crypto_payload_length)
    {}

    uint32_t Session::calc_max_crypto_payload_length(uint32_t max_link_payload_size)
    {
        return (max_link_payload_size > SessionData::min_size_bytes) ? max_link_payload_size - SessionData::min_size_bytes : 0;
    }

    bool Session::initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys, uint16_t nonce_start)
    {
        if (!keys.valid()) return false;

        this->statistics.num_init.increment();

        this->rx_valid = true;
        this->tx_valid = true;

        this->rx_nonce.set(nonce_start);
        this->tx_nonce.set(nonce_start);
        this->algorithms = algorithms;
        this->session_start = session_start;
        this->keys.copy(keys);

        return true;
    }

    void Session::reset()
    {
        this->statistics.num_reset.increment();
        this->rx_valid = false;
        this->tx_valid = false;
    }

    RSlice Session::validate_message(const SessionData& message, const openpal::Timestamp& now, std::error_code& ec)
    {
        if (!this->rx_valid)
        {
            this->statistics.num_user_data_without_session.increment();
            ec = CryptoError::no_valid_session;
            return RSlice::empty_slice();
        }

        auto dest = this->rx_auth_buffer.as_wslice();
        const auto payload = this->algorithms.mode->read(this->keys.rx_key, message, dest, ec);

        if (ec)
        {
            this->statistics.num_auth_fail.increment();
            return RSlice::empty_slice();
        }

        if (payload.is_empty())
        {
            ec = CryptoError::empty_user_data;
            this->statistics.num_auth_fail.increment();
            return RSlice::empty_slice();
        }

        const auto current_session_time = now.milliseconds - this->session_start.milliseconds;

        // the message is authentic, check the TTL
        if (current_session_time > message.metadata.valid_until_ms)
        {
            this->statistics.num_ttl_expiration.increment();
            ec = CryptoError::expired_ttl;
            return RSlice::empty_slice();
        }

        // check the nonce
        if (!this->algorithms.verify_nonce(this->rx_nonce.get(), message.metadata.nonce.value))
        {
            this->statistics.num_nonce_fail.increment();
            ec = CryptoError::invalid_rx_nonce;
            return RSlice::empty_slice();
        }

        this->rx_nonce.set(message.metadata.nonce.value);
        this->statistics.num_success.increment();

        return payload;
    }

    std::error_code Session::format_message(SessionData& msg, bool fir, const openpal::Timestamp& now, openpal::RSlice& cleartext)
    {
        const auto ret = this->format_message_impl(msg, fir, now, cleartext);
        if (ret)
        {
            // any error invalidates the tx direction of the session
            this->tx_valid = false;
        }
        return ret;
    }

    std::error_code Session::format_message_impl(SessionData& msg, bool fir, const openpal::Timestamp& now, openpal::RSlice& input)
    {
        if (!this->tx_valid)
        {
            return CryptoError::no_valid_session;
        }

        if (this->tx_nonce.is_max_value())
        {
            return CryptoError::invalid_tx_nonce;
        }

        const auto session_time_long = now.milliseconds - this->session_start.milliseconds;
        if (session_time_long > std::numeric_limits<uint32_t>::max())
        {
            return CryptoError::ttl_overflow;
        }

        const auto session_time = static_cast<uint32_t>(session_time_long);
        const auto remainder = std::numeric_limits<uint32_t>::max() - session_time;
        if (remainder < config.ttl_pad_ms)
        {
            return CryptoError::ttl_overflow;
        }

        // how big can the user data be?
        const uint16_t max_user_data_length = this->algorithms.mode->max_writable_user_data_length(this->max_crypto_payload_length);
        const auto fin = input.length() <= max_user_data_length;
        const auto user_data_length = fin ? input.length() : max_user_data_length;
        const auto user_data = input.take(user_data_length);

        // the metadata we're encoding
        AuthMetadata metadata(
            this->tx_nonce.get() + 1,
            session_time + config.ttl_pad_ms,
            SessionFlags(fir, fin)
        );

        std::error_code ec;
        const auto written_user_data = this->algorithms.mode->write(this->keys.tx_key, metadata, user_data, this->auth_tag_buffer, this->tx_encrypt_user_data_buffer.as_wslice(), ec);
        if (ec)
        {
            return ec;
        }

        msg.metadata = metadata;
        msg.user_data = Seq16(written_user_data);
        msg.auth_tag = Seq8(this->auth_tag_buffer.as_slice());

        // everything succeeded, so increment the nonce and advance the input buffer
        this->tx_nonce.increment();
        input.advance(user_data_length);

        return ec;
    }

}


