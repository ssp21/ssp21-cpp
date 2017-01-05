
#ifndef SSP21_RESPONDERHANDSHAKESTATES_H
#define SSP21_RESPONDERHANDSHAKESTATES_H

#include "ssp21/crypto/Responder.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class HandshakeIdle : public Responder::IHandshakeState, private openpal::Uncopyable
    {
        HandshakeIdle() {}

    public:

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeBegin& msg) override;

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeAuth& msg) override;

        static Responder::IHandshakeState& get()
        {
            static HandshakeIdle instance;
            return instance;
        }

    };

    class HandshakeWaitForAuth : public Responder::IHandshakeState, private openpal::Uncopyable
    {
        HandshakeWaitForAuth() {}

    public:

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeBegin& msg) override;

        virtual Responder::IHandshakeState& on_message(Responder& ctx, const seq32_t& msg_bytes, const RequestHandshakeAuth& msg) override;

        static Responder::IHandshakeState& get()
        {
            static HandshakeWaitForAuth instance;
            return instance;
        }

    };

}

#endif
