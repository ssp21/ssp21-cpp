
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
#include "ssp21/crypto/CryptoLayerConfig.h"

#include "ssp21/crypto/gen/SessionData.h"
#include "ssp21/IFrameWriter.h"

namespace ssp21
{
    class Session final : private openpal::Uncopyable
    {

    public:

        struct Param
        {
            Param() {}

            Param(const openpal::Timestamp& session_start, uint16_t max_nonce, uint32_t max_session_time) :
                session_start(session_start),
                max_nonce(max_nonce),
                max_session_time(max_session_time)
            {}

            // time of session initialization
            openpal::Timestamp session_start;

            // maximum allowed nonce value for receiving or transmitting
            uint16_t max_nonce = consts::crypto::initiator::default_max_nonce;

            // maximum allowed session time for receiving or transmitting
            uint32_t max_session_time = consts::crypto::initiator::default_max_session_time_ms;
        };

        Session(const std::shared_ptr<IFrameWriter>& frame_writer, const SessionConfig& config = SessionConfig());

        bool initialize(const Algorithms::Session& algorithms, const Param& parameters, const SessionKeys& keys);

        void reset();

        SessionStatistics get_statistics() const
        {
            return statistics;
        }

		seq32_t validate_session_auth(const SessionData& message, const openpal::Timestamp& now, std::error_code& ec);

        seq32_t validate_session_data(const SessionData& message, const openpal::Timestamp& now, std::error_code& ec);

        seq32_t format_session_message(bool fir, const openpal::Timestamp& now, seq32_t& cleartext, std::error_code& ec);

		// -------- getters -------------

        bool is_valid() const
        {
            return this->valid;
        }

        uint16_t get_rx_nonce() const
        {
            return rx_nonce.get();
        }

        uint16_t get_tx_nonce() const
        {
            return tx_nonce.get();
        }

    private:

		seq32_t validate_session_data_with_nonce_func(const SessionData& message, const openpal::Timestamp& now, verify_nonce_func_t verify, std::error_code& ec);

        bool valid = false;

        const std::shared_ptr<IFrameWriter> frame_writer;
        const SessionConfig config;

        /**
        * Given a maximum link layer payload, how big could the crypto payload be?
        */
        static constexpr uint32_t calc_max_crypto_payload_length(uint32_t max_link_payload_size);

        SessionStatistics statistics;

        // buffers used as scratch space for encryption/decyption operations
        openpal::Buffer decrypt_scratch_buffer;
        openpal::Buffer encrypt_scratch_buffer;

        Nonce rx_nonce;
        Nonce tx_nonce;

        SessionKeys keys;
        Algorithms::Session algorithms;
        Param parameters;

    };

}

#endif
