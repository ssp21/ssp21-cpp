
#include "ssp21/crypto/Session.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"
#include "ssp21/crypto/gen/CryptoError.h"

using namespace openpal;

namespace ssp21
{

    Session::Session(uint16_t max_rx_payload_size) : rx_auth_buffer(max_rx_payload_size)
    {}

    void Session::initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys)
    {
        this->statistics.num_init.increment();
        this->valid = true;
        this->rx_nonce = this->tx_nonce = 0;
        this->algorithms = algorithms;
        this->session_start = session_start;
        this->keys.copy(keys);
    }

    void Session::reset()
    {
        this->statistics.num_reset.increment();
        this->valid = false;
    }

    RSlice Session::validate_user_data(const UnconfirmedSessionData& message, const openpal::Timestamp& now, std::error_code& ec)
    {
        if (!this->valid)
        {
            this->statistics.num_user_data_without_session.increment();
            ec = CryptoError::no_valid_session;
            return RSlice::empty_slice();
        }

        auto dest = this->rx_auth_buffer.as_wslice();
        const auto payload = this->algorithms.read(this->keys.rx_key, message.metadata, message.payload, dest, ec);

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
        if (!this->algorithms.verify_nonce(this->rx_nonce, message.metadata.nonce.value))
        {
            this->statistics.num_nonce_fail.increment();
            ec = CryptoError::invalid_nonce;
            return RSlice::empty_slice();
        }

        this->rx_nonce = message.metadata.nonce.value;
        this->statistics.num_success.increment();

        return payload;
    }

}


