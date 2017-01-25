
#ifndef SSP21_CRYPTOLAYER_H
#define SSP21_CRYPTOLAYER_H

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/Handshake.h"
#include "ssp21/crypto/Reassembler.h"
#include "ssp21/crypto/TxState.h"

#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/ReplyHandshakeAuth.h"
#include "ssp21/crypto/gen/SessionData.h"

#include "ssp21/LayerInterfaces.h"
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

        struct Config
        {
            /// The maximum size of a reassembled message
            uint16_t max_reassembly_size = consts::link::max_config_payload_size;
        };

        CryptoLayer(
            HandshakeMode type,
            const Config& context_config,
            const Session::Config& session_config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const std::shared_ptr<const KeyPair>& local_static_key_pair,
            const std::shared_ptr<const PublicKey>& remote_static_public_key
        );

        /// ------ helper methods ------

        void bind_layers(ILowerLayer& lower, IUpperLayer& upper)
        {
            this->lower = &lower;
            this->upper = &upper;
        }

        // ---- final implementations from ILowerLayer -----

        virtual void receive() override final;

        virtual bool transmit(const seq32_t& data) override final;

    protected:

        // ----- final implementations from IUpperlayer ----

        virtual void on_close_impl() override final;

        virtual bool on_rx_ready_impl(const seq32_t& data) override final;

        virtual void on_tx_ready_impl() override final;

        // ------ methods to be overriden by super class ------

        // inherited class can perform additional cleanup when layer is closed
        virtual void reset_state_on_close() = 0;

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

        const std::shared_ptr<const KeyPair> local_static_key_pair;
        const std::shared_ptr<const PublicKey> remote_static_public_key;

        Handshake handshake;
        Session session;
        Reassembler reassembler;
        TxState tx_state;

        ILowerLayer* lower = nullptr;
        IUpperLayer* upper = nullptr;

    private:

        // ------ private helper methods ------

        void process(const seq32_t& message_data);

        inline bool can_receive() const
        {
            return this->lower->get_is_tx_ready() && !this->is_rx_ready;
        }

        inline bool can_transmit_session_data() const
        {
            /**
            * 1) The session must be valid
            * 2) Lower-layer must be ready to transmit
            * 3) transmission state must have some data to send
            */

            return this->session.is_valid() && this->lower->get_is_tx_ready() && this->tx_state.is_ready_tx();
        }

        void check_receive();

        void check_transmit();

        template <class MsgType>
        bool handle_message(const seq32_t& message, const openpal::Timestamp& now);
    };
}

#endif
