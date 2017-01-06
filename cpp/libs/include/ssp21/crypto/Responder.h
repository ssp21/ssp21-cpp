
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "ssp21/crypto/CryptoLayer.h"

namespace ssp21
{
    /**
    	Responder implementation - Inherits most of its functionality from the CryptoLayer base class.

    	Adds responder hand shaking support.
    */
    class Responder final : public CryptoLayer
    {
        friend class ResponderHandshakeIdle;
        friend class ResponderHandshakeWaitForAuth;

    public:

        struct Config : public CryptoLayer::Config {};

        struct IHandshakeState : private openpal::Uncopyable
        {
            virtual IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeBegin& msg) = 0;
            virtual IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeAuth& msg) = 0;
        };

        Responder(
            const Config& context_config,
            const Session::Config& session_config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            std::unique_ptr<KeyPair> local_static_key_pair,
            std::unique_ptr<PublicKey> remote_static_public_key
        );

        ResponderStatistics get_statistics() const
        {
            return ResponderStatistics(this->session.get_statistics());
        }

    private:

        // ---- private helper methods -----

        void reply_with_handshake_error(HandshakeError err);

        HandshakeError configure_feature_support(const RequestHandshakeBegin& msg);


        // ---- implement CryptoLayer -----

        virtual void reset_state_on_close() override;

        virtual bool supports(Function function) const override;

        virtual void on_parse_error(Function function, ParseError error) override;

        virtual bool on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual bool on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;


        // ---- private members -----

        // state instance for the handshake
        IHandshakeState* handshake_state;
    };

}

#endif
