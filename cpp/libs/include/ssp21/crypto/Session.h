
#ifndef SSP21_SESSION_H
#define SSP21_SESSION_H

#include "openpal/util/Uncopyable.h"
#include "openpal/logging/Logger.h"
#include "openpal/executor/Timestamp.h"
#include "openpal/container/Buffer.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/SessionModes.h"
#include "ssp21/crypto/Algorithms.h"
#include "ssp21/crypto/Statistics.h"

#include "ssp21/crypto/gen/UnconfirmedSessionData.h"

#include "ssp21/crypto/Constants.h"


namespace ssp21
{
    class Session final : private openpal::Uncopyable
    {

    public:

        struct Config
        {
            Config() {}

            uint16_t max_link_payload_size = consts::link::max_config_payload_size;            
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

        openpal::RSlice format_message(openpal::WSlice dest, bool fir, const openpal::Timestamp& now, openpal::RSlice& input, std::error_code& ec);

    private:

		/**
		* Given a maximum link layer payload, how big could the crypto payload be?
		*/
		static uint32_t max_crypto_payload_size(uint32_t max_link_payload_size);

		/**
		* Given a maximum link layer payload, what's the maximum amount of userdata we might receive?
		*/
		static uint32_t max_userdata_size(uint32_t max_link_payload_size);

        SessionStatistics statistics;
        Config config;

        openpal::Buffer rx_auth_buffer;
        openpal::Buffer tx_payload_buffer;

        bool valid = false;
        uint16_t rx_nonce = 0;
        uint16_t tx_nonce = 0;

        SessionKeys keys;
        Algorithms::Session algorithms;
        openpal::Timestamp session_start = openpal::Timestamp::min_value();

    };

}

#endif
