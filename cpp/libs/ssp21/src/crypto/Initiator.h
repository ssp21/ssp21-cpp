
#ifndef SSP21_INITIATOR_H
#define SSP21_INITIATOR_H

#include "crypto/CryptoLayer.h"
#include "crypto/IInitiatorHandshake.h"

#include "openpal/executor/TimerRef.h"

namespace ssp21
{
    /**
    	Initiator implementation - Inherits most of its functionality from the CryptoLayer base class.

    	Adds initiator hand shaking support and renegotiation support
    */
    class Initiator final : public CryptoLayer
    {

        friend struct InitiatorHandshakeStates;

    public:

        Initiator(
            const InitiatorConfig& config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const std::shared_ptr<IInitiatorHandshake>& handshake
        );

        class IHandshakeState : private openpal::Uncopyable
        {

        public:

            enum Enum
            {
                idle,
                wait_for_begin_reply,
                wait_for_auth_reply,
                wait_for_retry,
                bad_configuration
            };

            explicit IHandshakeState(Enum enum_value) : enum_value(enum_value)
            {}

            // called when conditions are met that require we renegotiate the session
            virtual IHandshakeState* on_handshake_required(Initiator& ctx, const openpal::Timestamp& now)
            {
                ctx.handshake_required = true; // defer renegotiation
                return this;
            }

            virtual IHandshakeState* on_reply_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now);
            virtual IHandshakeState* on_error_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now);
            virtual IHandshakeState* on_auth_message(Initiator& ctx, const SessionData& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now);

            // called when the response timeout timer fires
            virtual IHandshakeState* on_response_timeout(Initiator& ctx);

            // called when the retry timeout timer fires
            virtual IHandshakeState* on_retry_timeout(Initiator& ctx);

            const Enum enum_value;

        private:

            void log_unexpected_message(openpal::Logger& logger, Function func);

        };

        inline IHandshakeState::Enum get_state_enum() const
        {
            return this->handshake_state->enum_value;
        }

        struct Statistics
        {
            explicit Statistics(const SessionStatistics& session) : session(session) {}

            SessionStatistics session;
        };

    private:

        // ---- private helper methods -----

        void start_response_timer();

        void start_retry_timer();

        void on_handshake_required();

        // ---- final implementations from IUpperLayer ----

        virtual void on_lower_open_impl() override;

        // ---- implement CryptoLayer -----

        virtual void reset_state_on_close_from_lower() override;

        virtual bool supports(Function function) const override;

        virtual void on_session_nonce_change(uint16_t rx_nonce, uint16_t tx_nonce) override;

        virtual void on_pre_tx_ready() override;

        virtual void on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual void on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual void on_auth_session(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        // ---- private members -----

        const InitiatorConfig::Params params;
        const SessionLimits session_limits;

        IHandshakeState* handshake_state;
        const std::shared_ptr<IInitiatorHandshake> handshake;

        openpal::TimerRef response_and_retry_timer;
        openpal::TimerRef session_timeout_timer;

        bool handshake_required = false;
    };

}

#endif
