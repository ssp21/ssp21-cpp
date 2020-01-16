
#ifndef SSP21_SESSION_H
#define SSP21_SESSION_H

#include "exe4cpp/Typedefs.h"
#include "log4cpp/Logger.h"
#include "ser4cpp/container/Buffer.h"
#include "ser4cpp/util/Uncopyable.h"

#include "crypto/Algorithms.h"
#include "crypto/Nonce.h"
#include "crypto/SessionModes.h"
#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/Constants.h"
#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/Statistics.h"

#include "IFrameWriter.h"
#include "crypto/gen/SessionData.h"

namespace ssp21 {
class Session final : private ser4cpp::Uncopyable {

public:
    struct Param {
        Param() {}

        Param(const exe4cpp::steady_time_t& session_start, uint16_t max_nonce, const exe4cpp::duration_t& max_session_time)
            : session_start(session_start)
            , max_nonce(max_nonce)
            , max_session_time(max_session_time)
        {
        }

        // time of session initialization
        exe4cpp::steady_time_t session_start;

        // maximum allowed nonce value for receiving or transmitting
        uint16_t max_nonce = consts::crypto::initiator::default_max_nonce;

        // maximum allowed session time for receiving or transmitting
        exe4cpp::duration_t max_session_time = std::chrono::milliseconds(consts::crypto::initiator::default_max_session_time_ms);
    };

    Session(std::shared_ptr<IFrameWriter> frame_writer, std::shared_ptr<SessionStatistics> statistics, const SessionConfig& config = SessionConfig());

    bool initialize(const Algorithms::Session& algorithms, const Param& parameters, const SessionKeys& keys);

    void reset();

    seq32_t validate_session_auth(const SessionData& message, const exe4cpp::steady_time_t& now, wseq32_t dest, std::error_code& ec);

    seq32_t validate_session_data(const SessionData& message, const exe4cpp::steady_time_t& now, wseq32_t dest, std::error_code& ec);

    seq32_t format_session_data(const exe4cpp::steady_time_t& now, seq32_t& cleartext, std::error_code& ec);

    seq32_t format_session_auth(const exe4cpp::steady_time_t& now, seq32_t& cleartext, std::error_code& ec);

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

    exe4cpp::steady_time_t get_session_start() const
    {
        return this->parameters.session_start;
    }

private:
    seq32_t format_session_data_no_nonce_check(const exe4cpp::steady_time_t& now, seq32_t& cleartext, std::error_code& ec);

    seq32_t validate_session_data_with_nonce_func(const SessionData& message, const exe4cpp::steady_time_t& now, wseq32_t dest, verify_nonce_func_t verify, std::error_code& ec);

    /**
    *    Given a maximum link layer payload, how big could the crypto payload be?
    */
    static constexpr uint32_t calc_max_crypto_payload_length(uint32_t max_link_payload_size);

    bool valid = false;

    const std::shared_ptr<IFrameWriter> frame_writer;
    const std::shared_ptr<SessionStatistics> statistics;
    const SessionConfig config;

    Nonce rx_nonce;
    Nonce tx_nonce;

    SessionKeys keys;
    Algorithms::Session algorithms;
    Param parameters;
    ser4cpp::Buffer encrypt_buffer;
};

}

#endif
