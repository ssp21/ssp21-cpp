
#ifndef SSP21_RESPONDERHANDSHAKESTATES_H
#define SSP21_RESPONDERHANDSHAKESTATES_H

#include "ssp21/crypto/Responder.h"

namespace ssp21
{
    struct ResponderHandshake
    {
        class Idle final : public Responder::IHandshakeState
        {
            Idle() {}

        public:

            virtual Responder::IHandshakeState* on_message(Responder& ctx, const RequestHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;

            virtual Responder::IHandshakeState* on_message(Responder& ctx, const RequestHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;

            static Responder::IHandshakeState* get()
            {
                static Idle instance;
                return &instance;
            }

        };

        class WaitForAuth final : public Responder::IHandshakeState
        {
            WaitForAuth() {}

        public:

            virtual Responder::IHandshakeState* on_message(Responder& ctx, const RequestHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;

            virtual Responder::IHandshakeState* on_message(Responder& ctx, const RequestHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;

            static Responder::IHandshakeState* get()
            {
                static WaitForAuth instance;
                return &instance;
            }

        };
    };

}

#endif
