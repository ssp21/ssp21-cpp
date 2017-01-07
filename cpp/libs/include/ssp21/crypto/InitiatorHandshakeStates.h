
#ifndef SSP21_INITIATORHANDSHAKESTATES_H
#define SSP21_INITIATORHANDSHAKESTATES_H

#include "ssp21/crypto/Initiator.h"

namespace ssp21
{
    class InitiatorHandshakeIdle final : public Initiator::IHandshakeState
    {
        InitiatorHandshakeIdle() {}

    public:

        virtual IHandshakeState* on_handshake_required(Initiator& ctx, const openpal::Timestamp& now) override;

        static Initiator::IHandshakeState* get()
        {
            static InitiatorHandshakeIdle instance;
            return &instance;
        }
    };

    class InitiatorHandshakeWaitForBeginReply final : public Initiator::IHandshakeState
    {
        InitiatorHandshakeWaitForBeginReply() {}

    public:

        static Initiator::IHandshakeState* get()
        {
            static InitiatorHandshakeWaitForBeginReply instance;
            return &instance;
        }
    };

    class InitiatorHandshakeWaitForAuthReply final : public Initiator::IHandshakeState
    {
        InitiatorHandshakeWaitForAuthReply() {}

    public:

        static Initiator::IHandshakeState* get()
        {
            static InitiatorHandshakeWaitForAuthReply instance;
            return &instance;
        }
    };

    class InitiatorHandshakeWaitForRetry final : public Initiator::IHandshakeState
    {
        InitiatorHandshakeWaitForRetry() {}

    public:

        static Initiator::IHandshakeState* get()
        {
            static InitiatorHandshakeWaitForRetry instance;
            return &instance;
        }
    };

    class InitiatorHandshakeBadConfiguration final : public Initiator::IHandshakeState
    {
        InitiatorHandshakeBadConfiguration() {}

    public:

        static Initiator::IHandshakeState* get()
        {
            static InitiatorHandshakeBadConfiguration instance;
            return &instance;
        }
    };

}

#endif
