
#ifndef SSP21_CRYPTOLAYER_H
#define SSP21_CRYPTOLAYER_H

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/Handshake.h"
#include "ssp21/crypto/Reassembler.h"
#include "ssp21/crypto/TxState.h"
#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/crypto/CryptoLayerConfig.h"

#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/ReplyHandshakeAuth.h"
#include "ssp21/crypto/gen/SessionData.h"

#include "ssp21/stack/ILowerLayer.h"
#include "ssp21/stack/IUpperLayer.h"
#include "ssp21/IFrameWriter.h"

#include "openpal/executor/IExecutor.h"

namespace ssp21
{
    /**
    * Common base class for initiator and responder
    */
    class CryptoLayer : public IUpperLayer, public ILowerLayer
    {

    public:

        CryptoLayer(
            HandshakeMode type,
            const CryptoLayerConfig& context_config,
            const SessionConfig& session_config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const StaticKeys& static_keys,
            const std::shared_ptr<ICertificateHandler>& certificate_handler
        );

        void bind(ILowerLayer& lower, IUpperLayer& upper)
        {
            this->lower = &lower;
            this->upper = &upper;
        }

        // ---- final implementations from ILowerLayer -----

        virtual bool start_tx_from_upper(const seq32_t& data) override final;

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

        // optional overrides for each type of message
        virtual void on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) {}

        virtual void on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) {}

        virtual void on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) {}

        virtual void on_message(const ReplyHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) {}

        virtual void on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const openpal::Timestamp& now) {}


        // non-virtual b/c both sides implement it the same way
        void on_message(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now);

        /// ------ member variables ------

        openpal::Logger logger;

        const std::shared_ptr<IFrameWriter> frame_writer;
        const std::shared_ptr<openpal::IExecutor> executor;

        const StaticKeys static_keys;
        const std::shared_ptr<ICertificateHandler> certificate_handler;

        Handshake handshake;
        Session session;
        Reassembler reassembler;
        TxState tx_state;

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

            return this->session.is_valid() && this->lower->is_tx_ready() && this->tx_state.is_ready_tx();
        }

        void check_transmit();

        template <class MsgType>
        bool handle_message(const seq32_t& message, const openpal::Timestamp& now);
    };
}

#endif
