
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "crypto/CryptoLayer.h"

#include "crypto/IResponderHandshake.h"

namespace ssp21 {
/**
    	Responder implementation - Inherits most of its functionality from the CryptoLayer base class.

    	Adds responder hand shaking support.
    */
class Responder final : public CryptoLayer {

public:
    Responder(
        const ResponderConfig& config,
        const log4cpp::Logger& logger,
        const std::shared_ptr<IFrameWriter>& frame_writer,
        const std::shared_ptr<exe4cpp::IExecutor>& executor,
        const std::shared_ptr<IResponderHandshake>& handshake);

private:
    const std::shared_ptr<IResponderHandshake> handshake;

    // ---- final implementations from IUpperLayer ----

    virtual void on_lower_open_impl() override {}

    // ---- private helper methods -----

    void reply_with_handshake_error(HandshakeError err);

    // ---- implement CryptoLayer -----

    virtual void reset_state_on_close_from_lower() override;

    virtual bool supports(Function function) const override;

    virtual void on_parse_error(Function function, ParseError error) override;

    virtual void on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now) override;

    virtual void on_auth_session(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now) override;
};

}

#endif
