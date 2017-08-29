
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

    public:

        Responder(
            const ResponderConfig& config,
            const openpal::Logger& logger,
            const std::shared_ptr<IFrameWriter>& frame_writer,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const StaticKeys& static_keys,
            const std::shared_ptr<ICertificateHandler>& certificate_handler
        );

        inline ResponderStatistics get_statistics() const
        {
            return ResponderStatistics(this->sessions.active->get_statistics());
        }

    private:

        // ---- final implementations from IUpperLayer ----

        virtual void on_lower_open_impl() override {}

        // ---- private helper methods -----

        void reply_with_handshake_error(HandshakeError err);

        HandshakeError verify_handshake_begin(const RequestHandshakeBegin& msg, Algorithms& algorithms, seq32_t& public_key_out);


        // ---- implement CryptoLayer -----

        virtual void reset_state_on_close_from_lower() override;

        virtual bool supports(Function function) const override;

        virtual void on_parse_error(Function function, ParseError error) override;

        virtual void on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

        virtual void on_auth_session(const SessionData& msg, const seq32_t& raw_data, const openpal::Timestamp& now) override;

    };

}

#endif
