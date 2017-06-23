
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
        friend struct ResponderHandshake;

    public:

        struct IHandshakeState : private openpal::Uncopyable
        {
            enum Enum
            {
                idle,
                wait_for_auth
            };

            explicit IHandshakeState(Enum enum_value) : enum_value(enum_value)
            {}

            virtual IHandshakeState* on_message(Responder& ctx, const RequestHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) = 0;
            virtual IHandshakeState* on_message(Responder& ctx, const RequestHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) = 0;

            const Enum enum_value;
        };

        Responder(
            const ResponderConfig& config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const LocalKeys& keys,
            const std::shared_ptr<ICertificateHandler>& certificate_handler
        );

        inline ResponderStatistics get_statistics() const
        {
            return ResponderStatistics(this->session.get_statistics());
        }

        inline IHandshakeState::Enum get_state_enum() const
        {
            return this->handshake_state->enum_value;
        }

    private:

        // ---- final implementations from IUpperLayer ----

        virtual void on_lower_open_impl() override {}

        // ---- private helper methods -----

        void reply_with_handshake_error(HandshakeError err);

        HandshakeError verify_handshake_begin(const RequestHandshakeBegin& msg, seq32_t& public_key_out);


        // ---- implement CryptoLayer -----

        virtual void reset_state_on_close_from_lower() override;

        virtual bool supports(Function function) const override;

        virtual void on_parse_error(Function function, ParseError error) override;

        virtual void on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual void on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;


        // ---- private members -----

        // state instance for the handshake
        IHandshakeState* handshake_state;
    };

}

#endif
