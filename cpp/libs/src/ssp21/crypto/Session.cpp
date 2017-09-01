
#include "ssp21/crypto/Session.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/stack/LogLevels.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include <limits>

using namespace openpal;

namespace ssp21
{

    Session::Session(const std::shared_ptr<IFrameWriter>& frame_writer, const std::shared_ptr<SessionStatistics>& statistics, const SessionConfig& config) :
        frame_writer(frame_writer),
		statistics(statistics),
        config(config)
    {}

    constexpr uint32_t Session::calc_max_crypto_payload_length(uint32_t max_link_payload_size)
    {
        return (max_link_payload_size > SessionData::min_size_bytes) ? max_link_payload_size - SessionData::min_size_bytes : 0;
    }

    bool Session::initialize(const Algorithms::Session& algorithms, const Param& parameters, const SessionKeys& keys)
    {
        if (!keys.valid()) return false;

        this->rx_nonce.set(0);
        this->tx_nonce.set(0);
        this->algorithms = algorithms;
        this->parameters = parameters;
        this->keys.copy(keys);

        this->valid = true;

        return true;
    }

    void Session::reset()
    {
        this->valid = false;
        this->keys.zero();        
    }

    seq32_t Session::validate_session_auth(const SessionData& message, const openpal::Timestamp& now, wseq32_t dest, std::error_code& ec)
    {
        return this->validate_session_data_with_nonce_func(message, now, dest, NonceFunctions::equal_to_zero, ec);
    }

    seq32_t Session::validate_session_data(const SessionData& message, const openpal::Timestamp& now, wseq32_t dest, std::error_code& ec)
    {
        const auto payload = this->validate_session_data_with_nonce_func(message, now, dest, this->algorithms.verify_nonce, ec);
		
		if (ec) {
			return payload;
		}

		if (payload.is_empty())
		{
			ec = CryptoError::empty_user_data;			
		}

		return payload;
    }

    seq32_t Session::format_session_auth(const openpal::Timestamp& now, seq32_t& cleartext, std::error_code& ec)
    {
        if (!this->tx_nonce.is_zero())
        {
            ec = CryptoError::max_nonce_exceeded;
            return seq32_t::empty();
        }

        return this->format_session_data(now, cleartext, 0, ec);
    }

    seq32_t Session::format_session_data(const openpal::Timestamp& now, seq32_t& cleartext, std::error_code& ec)
    {
        if (this->tx_nonce.get() >= this->parameters.max_nonce)
        {
            ec = CryptoError::max_nonce_exceeded;
            return seq32_t::empty();
        }

        return this->format_session_data(now, cleartext, this->tx_nonce.get() + 1, ec);
    }

    seq32_t Session::validate_session_data_with_nonce_func(const SessionData& message, const openpal::Timestamp& now, wseq32_t dest, verify_nonce_func_t verify_nonce, std::error_code& ec)
    {
        if (!this->valid)
        {            
            ec = CryptoError::no_valid_session;
            return seq32_t::empty();
        }

        const auto payload = this->algorithms.session_mode->read(this->keys.rx_key, message, dest, ec);

        if (ec)
        {            
            return seq32_t::empty();
        }        

        if (now.milliseconds < this->parameters.session_start.milliseconds)
        {
            ec = CryptoError::clock_rollback;
            return seq32_t::empty();
        }

        const auto current_session_time = now.milliseconds - this->parameters.session_start.milliseconds;

        if (current_session_time > this->parameters.max_session_time)
        {
            ec = CryptoError::max_session_time_exceeded;
            return seq32_t::empty();
        }

        // the message is authentic, check the TTL
        if (current_session_time > message.metadata.valid_until_ms)
        {            
            ec = CryptoError::expired_ttl;
            return seq32_t::empty();
        }

        // check the nonce via the configured maximum
        if (message.metadata.nonce > this->parameters.max_nonce)
        {            
            ec = CryptoError::max_nonce_exceeded;
            return seq32_t::empty();
        }

        // check the nonce via the verification function
        if (!verify_nonce(this->rx_nonce.get(), message.metadata.nonce))
        {            
            ec = CryptoError::nonce_replay;
            return seq32_t::empty();
        }

        this->rx_nonce.set(message.metadata.nonce.value);        

        return payload;
    }

    seq32_t Session::format_session_data(const openpal::Timestamp& now, seq32_t& clear_text, uint16_t nonce, std::error_code& ec)
    {
        if (!this->valid)
        {
            ec = CryptoError::no_valid_session;
            return seq32_t::empty();
        }

        if (now.milliseconds < this->parameters.session_start.milliseconds)
        {
            ec = CryptoError::clock_rollback;
            return seq32_t::empty();
        }

        const auto session_time_long = now.milliseconds - this->parameters.session_start.milliseconds;

        if (session_time_long > this->parameters.max_session_time)
        {
            ec = CryptoError::max_session_time_exceeded;
            return seq32_t::empty();
        }

        const auto session_time = static_cast<uint32_t>(session_time_long);				// safe downcast since session_time_long <= the uin32_t above
        const auto remainder = this->parameters.max_session_time - session_time;	    // safe substract since session_time > config.max_session_time
        if (remainder < config.ttl_pad_ms)
        {
            ec = CryptoError::max_session_time_exceeded;
            return seq32_t::empty();
        }

        // the metadata we're encoding
        const AuthMetadata metadata(
            nonce,
            session_time + config.ttl_pad_ms
        );

        const auto frame = this->algorithms.session_mode->write(*this->frame_writer, this->keys.tx_key, metadata, clear_text, ec);
        if (ec)
        {
            return seq32_t::empty();
        }

        // everything succeeded, so increment the nonce
        this->tx_nonce.increment();

        return frame;
    }

}


