
#ifndef SSP21_RESPONDERHANDSHAKESTATES_H
#define SSP21_RESPONDERHANDSHAKESTATES_H

#include "ssp21/crypto/Responder.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class ResponderHandshakeIdle : public Responder::IHandshakeState, private openpal::Uncopyable
    {
        ResponderHandshakeIdle() {}

    public:

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeBegin& msg) override;

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeAuth& msg) override;

        static Responder::IHandshakeState& get()
        {
            static ResponderHandshakeIdle instance;
            return instance;
        }

    };

    class ResponderHandshakeWaitForAuth : public Responder::IHandshakeState, private openpal::Uncopyable
    {
        ResponderHandshakeWaitForAuth() {}

    public:

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeBegin& msg) override;

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeAuth& msg) override;

        static Responder::IHandshakeState& get()
        {
            static ResponderHandshakeWaitForAuth instance;
            return instance;
        }

    };

}

#endif
