
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

#include "ssp21/crypto/gen/SessionData.h"
#include "ssp21/IFrameWriter.h"

namespace ssp21
{
    class Session final : private openpal::Uncopyable
    {

    public:

        struct Config
        {
            Config() {}

            Config(uint32_t ttl_pad_ms, uint16_t max_nonce, uint32_t max_session_time) :
                ttl_pad_ms(ttl_pad_ms),
                max_nonce(max_nonce),
                max_session_time(max_session_time)
            {}

            // the TTL padding added to the current session time of every message
            uint32_t ttl_pad_ms = consts::crypto::default_ttl_pad_ms;

            // maximum allowed nonce value for receiving or transmitting
            uint16_t max_nonce = consts::crypto::default_max_nonce;

            // maximum allowed session time for receiving or transmitting
            uint32_t max_session_time = consts::crypto::default_max_session_time_ms;
        };

        Session(const std::shared_ptr<IFrameWriter>& frame_writer, const Config& config = Config());

        bool initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys);

        void reset();

        SessionStatistics get_statistics() const
        {
            return statistics;
        }

        seq32_t validate_message(const SessionData& message, const openpal::Timestamp& now, std::error_code& ec);

        seq32_t format_session_message(bool fir, const openpal::Timestamp& now, seq32_t& cleartext, std::error_code& ec);

        bool is_valid() const
        {
            return this->valid;
        }

    private:

        bool valid = false;

        const std::shared_ptr<IFrameWriter> frame_writer;
        const Config config;

        /**
        * Given a maximum link layer payload, how big could the crypto payload be?
        */
        static uint32_t calc_max_crypto_payload_length(uint32_t max_link_payload_size);

        SessionStatistics statistics;

        // buffers used as scratch space for encryption/decyption operations
        openpal::Buffer decrypt_scratch_buffer;
        openpal::Buffer encrypt_scratch_buffer;

        Nonce rx_nonce;
        Nonce tx_nonce;

        SessionKeys keys;
        Algorithms::Session algorithms;
        openpal::Timestamp session_start = openpal::Timestamp::min_value();

    };

}

#endif
