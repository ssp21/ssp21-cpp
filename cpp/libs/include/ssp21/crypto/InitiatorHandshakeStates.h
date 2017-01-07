
#ifndef SSP21_INITIATORHANDSHAKESTATES_H
#define SSP21_INITIATORHANDSHAKESTATES_H

#include "ssp21/crypto/Initiator.h"

namespace ssp21
{
    class InitiatorHandshakeStateIdle final : public Initiator::IHandshakeState
    {
        InitiatorHandshakeStateIdle() {}

    public:

        virtual IHandshakeState& on_handshake_required(Initiator& ctx, const openpal::Timestamp& now) override;

        static Initiator::IHandshakeState& get()
        {
            static InitiatorHandshakeStateIdle instance;
            return instance;
        }
    };

}

#endif
