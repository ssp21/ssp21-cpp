
#ifndef SSP21_INITIATORHANDSHAKESTATES_H
#define SSP21_INITIATORHANDSHAKESTATES_H

#include "ssp21/crypto/Initiator.h"

namespace ssp21
{
    class InitiatorHandshakeStateIdle final : public Initiator::IHandshakeState
    {
        InitiatorHandshakeStateIdle() {}

    public:

        virtual Initiator::IHandshakeState& on_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;
        virtual Initiator::IHandshakeState& on_message(Initiator& ctx, const ReplyHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;
        virtual Initiator::IHandshakeState& on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;
        virtual Initiator::IHandshakeState& on_response_timeout(Initiator& ctx) override;

        static Initiator::IHandshakeState& get()
        {
            static InitiatorHandshakeStateIdle instance;
            return instance;
        }
    };

}

#endif
