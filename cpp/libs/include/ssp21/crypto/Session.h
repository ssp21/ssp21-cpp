
#ifndef SSP21_SESSION_H
#define SSP21_SESSION_H

#include "openpal/util/Uncopyable.h"
#include "openpal/logging/Logger.h"
#include "openpal/executor/Timestamp.h"
#include "openpal/container/Buffer.h"

#include "ssp21/crypto/Nonce.h"
#include "ssp21/crypto/Constants.h"
#include "ssp21/crypto/Algorithms.h"
#include "ssp21/crypto/Statistics.h"
#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/SessionModes.h"

#include "ssp21/crypto/gen/UnconfirmedSessionData.h"

namespace ssp21
{
    class Session final : private openpal::Uncopyable
    {

    public:

        struct Config
        {
            Config() {}

            uint16_t max_link_payload_length = consts::link::max_config_payload_size;
            uint32_t ttl_pad_ms = consts::crypto::default_ttl_pad_ms;
        };

        explicit Session(const Config& config = Config());

        bool initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys, uint16_t nonce_start = 0);

        void reset();

        SessionStatistics get_statistics() const
        {
            return statistics;
        }

        openpal::RSlice validate_message(const UnconfirmedSessionData& message, const openpal::Timestamp& now, std::error_code& ec);

        std::error_code format_message(UnconfirmedSessionData& msg, bool fir, const openpal::Timestamp& now, openpal::RSlice& cleartext);

        bool can_transmit() const
        {
            return this->tx_valid && !tx_nonce.is_max_value();
        }

    private:

        std::error_code format_message_impl(UnconfirmedSessionData& msg, bool fir, const openpal::Timestamp& now, openpal::RSlice& cleartext);

        /**
        * Given a maximum link layer payload, how big could the crypto payload be?
        */
        static uint32_t calc_max_crypto_payload_length(uint32_t max_link_payload_size);

        SessionStatistics statistics;
        Config config;

        // calculated during construction
        const uint16_t max_crypto_payload_length;

        openpal::Buffer rx_auth_buffer;
        openpal::Buffer tx_payload_buffer;

        bool rx_valid = false;
        bool tx_valid = false;

        Nonce rx_nonce;
        Nonce tx_nonce;

        SessionKeys keys;
        Algorithms::Session algorithms;
        openpal::Timestamp session_start = openpal::Timestamp::min_value();

    };

}

#endif
