
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

            uint16_t max_rx_user_data_size = consts::crypto::default_session_max_rx_userdata_size;
            uint16_t max_tx_payload_size = consts::crypto::default_session_max_tx_payload_size;
            uint32_t ttl_pad_ms = consts::crypto::default_ttl_pad_ms;
        };

        Session(const Config& config = Config());

        bool initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys, uint16_t nonce_start = 0);

        void reset();

        SessionStatistics get_statistics() const
        {
            return statistics;
        }

        openpal::RSlice validate_user_data(const UnconfirmedSessionData& message, const openpal::Timestamp& now, std::error_code& ec);

        bool format_tx_message(UnconfirmedSessionData& msg, bool fir, const openpal::Timestamp& now, openpal::RSlice& input, std::error_code& ec);

    private:

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
