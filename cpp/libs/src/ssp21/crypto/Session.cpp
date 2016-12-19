
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
        rx_auth_buffer(max_crypto_payload_size(config.max_link_payload_size)),
        tx_payload_buffer(max_userdata_size(config.max_link_payload_size))
    {}

	uint32_t Session::max_crypto_payload_size(uint32_t max_link_payload_size)
	{
		return (max_link_payload_size > UnconfirmedSessionData::min_size_bytes) ? max_link_payload_size - UnconfirmedSessionData::min_size_bytes : 0;
	}

	uint32_t Session::max_userdata_size(uint32_t max_link_payload_size)
	{
		const auto base_size = UnconfirmedSessionData::min_size_bytes + consts::crypto::max_session_auth_tag_length;

		return (max_link_payload_size > base_size) ? max_link_payload_size - base_size : 0;
	}

    bool Session::initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys, uint16_t nonce_start)
    {
        if (!keys.valid()) return false;

        this->statistics.num_init.increment();
        this->valid = true;
        this->rx_nonce = this->tx_nonce = nonce_start;
        this->algorithms = algorithms;
        this->session_start = session_start;
        this->keys.copy(keys);

        return true;
    }

    void Session::reset()
    {
        this->statistics.num_reset.increment();
        this->valid = false;
    }

    RSlice Session::validate_message(const UnconfirmedSessionData& message, const openpal::Timestamp& now, std::error_code& ec)
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
            ec = CryptoError::invalid_rx_nonce;
            return RSlice::empty_slice();
        }

        this->rx_nonce = message.metadata.nonce.value;
        this->statistics.num_success.increment();

        return payload;
    }

	RSlice Session::format_message(openpal::WSlice dest, bool fir, const openpal::Timestamp& now, openpal::RSlice& input, std::error_code& ec)
    {
        if (!this->valid)
        {
            ec = CryptoError::no_valid_session;
			return RSlice::empty_slice();
        }

        if (this->tx_nonce == std::numeric_limits<uint16_t>::max())
        {
            ec = CryptoError::invalid_tx_nonce;
			return RSlice::empty_slice();
        }

        const auto session_time_long = now.milliseconds - this->session_start.milliseconds;
        if (session_time_long > std::numeric_limits<uint32_t>::max())
        {
            ec = CryptoError::ttl_overflow;
			return RSlice::empty_slice();
        }

        const auto session_time = static_cast<uint32_t>(session_time_long);
        const auto remainder = std::numeric_limits<uint32_t>::max() - session_time;
        if (remainder < config.ttl_pad_ms)
        {
            ec = CryptoError::ttl_overflow;
			return RSlice::empty_slice();
        }

        // how big can the user data be?
		const auto max_userdata_length = this->tx_payload_buffer.length() - consts::crypto::max_session_auth_tag_length;
		const auto fin = input.length() <= max_userdata_length;
		const auto userdata_length = fin ? input.length() : max_userdata_length;
		const auto userdata = input.take(userdata_length);

		// the metadata we're encoding
		AuthMetadata metadata(
			this->tx_nonce + 1,
			session_time + config.ttl_pad_ms,
			SessionFlags(fir, fin)
		);
			
		const auto payload = this->algorithms.write(this->keys.tx_key, metadata, userdata, dest, ec);
		if (ec)
		{
			return RSlice::empty_slice();
		}

		UnconfirmedSessionData msg(
			metadata,
			Seq16(payload)
		);

		const auto res = msg.write(dest);

		if (res.is_error())
		{
			ec = FormatError::insufficient_space;
			return RSlice::empty_slice();
		}		

		// everything succeeded, so increment the nonce an return
        this->rx_nonce++;		

		return res.written;
    }

}


