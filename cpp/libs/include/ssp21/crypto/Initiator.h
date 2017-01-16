
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

        friend struct InitiatorHandshake;

    public:

        struct Config : public CryptoLayer::Config
        {
            struct Suite
            {
                NonceMode nonce_mode = NonceMode::increment_last_rx;
                DHMode dh_mode = DHMode::x25519;
                HandshakeHash handshake_hash = HandshakeHash::sha256;
                HandshakeKDF handshake_kdf = HandshakeKDF::hkdf_sha256;
                HandshakeMAC handshake_mac = HandshakeMAC::hmac_sha256;
                SessionMode session_mode = SessionMode::hmac_sha256_16;
            };

            struct Params
            {
                openpal::TimeDuration response_timeout = openpal::TimeDuration::seconds(2);
                openpal::TimeDuration retry_timeout = openpal::TimeDuration::seconds(5);
				uint32_t max_session_time_ms = consts::crypto::default_max_session_time_ms;
				uint16_t max_nonce_value = consts::crypto::default_max_nonce;
            };

            Suite suite;
            Params params;
        };

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

            // called when the retry timeout timer fires
            virtual IHandshakeState* on_retry_timeout(Initiator& ctx)
            {
                return this;
            }

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

        Statistics get_statistics() const
        {
            return Statistics(session.get_statistics());
        }

    private:

        // ---- private helper methods -----

        void start_response_timer();

        void start_retry_timer();

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

        const Config::Suite suite;
        const Config::Params params;

        openpal::TimerRef response_and_retry_timer;
        openpal::Timestamp request_handshake_begin_time_tx;
    };

}

#endif
