
#ifndef SSP21_INITIATOR_H
#define SSP21_INITIATOR_H

#include "ssp21/crypto/CryptoLayer.h"

#include "openpal/executor/TimerRef.h"

namespace ssp21
{
    /**
    	Initiator implementation - Inherits most of its functionality from the CryptoLayer base class.

    	Adds initiator hand shaking support and renegotiation support
    */
    class Initiator final : public CryptoLayer
    {

    public:

        struct Config : public CryptoLayer::Config {};

        Initiator(
            const Config& context_config,
            const Session::Config& session_config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            std::unique_ptr<KeyPair> local_static_key_pair,
            std::unique_ptr<PublicKey> remote_static_public_key
        );

        class IHandshakeState : private openpal::Uncopyable
        {

        public:
            // called when conditions are met that require we renegotiate the session
            virtual IHandshakeState* on_handshake_required(Initiator& ctx, const openpal::Timestamp& now)
            {
                return this;
            }

            virtual IHandshakeState* on_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now);
            virtual IHandshakeState* on_message(Initiator& ctx, const ReplyHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now);
            virtual IHandshakeState* on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now);

            // called when the response timeout timer fires
            virtual IHandshakeState* on_response_timeout(Initiator& ctx)
            {
                return this;
            }

        private:

            void log_unexpected_message(openpal::Logger& logger, Function func);

        };

    private:

        // ---- final implementations from IUpperLayer ----

        virtual void on_open_impl() override;

        // ---- implement CryptoLayer -----

        virtual void reset_state_on_close() override;

        virtual bool supports(Function function) const override;

        virtual void on_parse_error(Function function, ParseError error) override {}

        virtual void on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual void on_message(const ReplyHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual void on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        // ---- private members -----

        IHandshakeState* handshake_state;

        openpal::TimerRef response_timer;
    };

}

#endif
