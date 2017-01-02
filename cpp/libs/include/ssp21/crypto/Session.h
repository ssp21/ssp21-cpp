
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

            // the TTL padding added to every message
            uint32_t ttl_pad_ms = consts::crypto::default_ttl_pad_ms;

            // maximum allowed nonce value for rx or tx
            uint16_t max_nonce = consts::crypto::default_max_nonce;
        };

        explicit Session(const std::shared_ptr<IFrameWriter>& frame_writer, const Config& config = Config());

        bool initialize(const Algorithms::Session& algorithms, const openpal::Timestamp& session_start, const SessionKeys& keys, uint16_t nonce_start = 0);

        void reset();

        SessionStatistics get_statistics() const
        {
            return statistics;
        }

        seq32_t validate_message(const SessionData& message, const openpal::Timestamp& now, std::error_code& ec);

        seq32_t format_session_message(bool fir, const openpal::Timestamp& now, seq32_t& cleartext, std::error_code& ec);

        bool can_transmit() const
        {
            return this->valid && !tx_nonce.is_max_value();
        }

    private:

        seq32_t format_session_message_impl(bool fir, const openpal::Timestamp& now, seq32_t& cleartext, std::error_code& ec);

        /**
        * Given a maximum link layer payload, how big could the crypto payload be?
        */
        static uint32_t calc_max_crypto_payload_length(uint32_t max_link_payload_size);

        SessionStatistics statistics;

        const std::shared_ptr<IFrameWriter> frame_writer;
        const Config config;

        // buffers used as scratch space for encryption/decyption operations
        openpal::Buffer decrypt_scratch_buffer;
        openpal::Buffer encrypt_scratch_buffer;

        bool valid = false;

        Nonce rx_nonce;
        Nonce tx_nonce;

        SessionKeys keys;
        Algorithms::Session algorithms;
        openpal::Timestamp session_start = openpal::Timestamp::min_value();

    };

}

#endif
