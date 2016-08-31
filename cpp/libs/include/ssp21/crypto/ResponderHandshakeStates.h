
#ifndef SSP21_RESPONDERHANDSHAKESTATES_H
#define SSP21_RESPONDERHANDSHAKESTATES_H

#include "ssp21/crypto/Responder.h"

#include "openpal/util/Uncopyable.h"

namespace ssp21
{

    class HandshakeIdle : public Responder::IHandshakeState, private openpal::Uncopyable
    {
        HandshakeIdle() {}

        static HandshakeIdle instance;

    public:

        virtual Responder::IHandshakeState& on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeBegin& msg) override;

        virtual Responder::IHandshakeState& on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeAuth& msg) override;

        static Responder::IHandshakeState& get()
        {
            return instance;
        }

    };

    class HandshakeWaitForAuth : public Responder::IHandshakeState, private openpal::Uncopyable
    {
        HandshakeWaitForAuth() {}

        static HandshakeWaitForAuth instance;

    public:

        virtual Responder::IHandshakeState& on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeBegin& msg) override;

        virtual Responder::IHandshakeState& on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeAuth& msg) override;

        static Responder::IHandshakeState& get()
        {
            return instance;
        }

    };

}

#endif
