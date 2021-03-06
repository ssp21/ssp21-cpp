
#ifndef SSP21_CRYPTOLAYER_H
#define SSP21_CRYPTOLAYER_H

#include "crypto/Sessions.h"
#include "crypto/TxState.h"
#include "ssp21/crypto/CryptoLayerConfig.h"

#include "crypto/gen/ReplyHandshakeBegin.h"
#include "crypto/gen/ReplyHandshakeError.h"
#include "crypto/gen/RequestHandshakeBegin.h"
#include "crypto/gen/SessionData.h"

#include "IFrameWriter.h"
#include "ssp21/stack/ILowerLayer.h"
#include "ssp21/stack/IUpperLayer.h"

#include "exe4cpp/IExecutor.h"
#include "ssp21/util/SecureDynamicBuffer.h"

namespace ssp21 {
/**
    * Common base class for initiator and responder
    */
class CryptoLayer : public IUpperLayer, public ILowerLayer {

public:
    CryptoLayer(
        const CryptoLayerConfig& context_config,
        const SessionConfig& session_config,
        const log4cpp::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<exe4cpp::IExecutor>& executor);

    void bind(ILowerLayer& lower, IUpperLayer& upper)
    {
        this->lower = &lower;
        this->upper = &upper;
    }

    // ---- final implementations from ILowerLayer -----

    virtual bool start_tx_from_upper(const seq32_t& data) override final;

    inline const SessionStatistics& get_statistics() const
    {
        return *this->statistics;
    }

protected:
    virtual bool is_tx_ready() const override final;

    virtual seq32_t start_rx_from_upper_impl() override final;

    virtual void discard_rx_data() override final;

    // ----- final implementations from IUpperlayer ----

    virtual void on_lower_close_impl() override final;

    virtual void on_lower_tx_ready_impl() override final;

    virtual void on_lower_rx_ready_impl() override final;

    // ------ methods to be overriden by super class ------

    // inherited class can perform additional cleanup when layer is closed
    virtual void reset_state_on_close_from_lower() = 0;

    // should a paritcular function code even be parsed?
    virtual bool supports(Function function) const = 0;

    // Called when a parse error occurs for a particular message type
    virtual void on_parse_error(Function function, ParseError error) {}

    // Called prior to checking checking if session data should be transmitted
    virtual void on_pre_tx_ready() {}

    // Called when either the rx or tx nonces change
    virtual void on_session_nonce_change(uint16_t rx_nonce, uint16_t tx_nonce) {}

    // optional overrides for optional messages
    virtual void on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now) {}
    virtual void on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now) {}
    virtual void on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now) {}

    // non-virtual b/c both sides implement it the same way
    void on_message(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now);
    void on_session_data(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now);

    // both parties implement this in different ways
    virtual void on_auth_session(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now) = 0;

    // both parties need to call this to complete the handshake at different times
    bool transmit_session_auth(Session& session);

    // ------ member variables ------

    log4cpp::Logger logger;

    const std::shared_ptr<IFrameWriter> frame_writer;
    const std::shared_ptr<exe4cpp::IExecutor> executor;
    const std::shared_ptr<SessionStatistics> statistics;
    Sessions sessions;

    TxState tx_state;
    SecureDynamicBuffer payload_buffer;
    seq32_t payload_data;

    ILowerLayer* lower = nullptr;
    IUpperLayer* upper = nullptr;

private:
    void try_read_from_lower();

    bool try_read_one_from_lower();

    // ------ private helper methods ------

    void process(const seq32_t& message_data);

    inline bool can_transmit_session_data() const
    {
        /**
            * 1) The session must be valid
            * 2) Lower-layer must be ready to transmit
            * 3) transmission state must have some data to send
            */

        return this->sessions.active->is_valid() && this->lower->is_tx_ready() && this->tx_state.is_ready_tx();
    }

    void check_transmit();

    template <class MsgType>
    bool handle_message(const seq32_t& message, const exe4cpp::steady_time_t& now);
};
}

#endif
