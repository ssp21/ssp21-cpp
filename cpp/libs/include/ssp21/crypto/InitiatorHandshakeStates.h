
#ifndef SSP21_INITIATORHANDSHAKESTATES_H
#define SSP21_INITIATORHANDSHAKESTATES_H

#include "ssp21/crypto/Initiator.h"

namespace ssp21
{
    struct InitiatorHandshake
    {
        class Idle final : public Initiator::IHandshakeState
        {
            Idle() {}

        public:

            virtual IHandshakeState* on_handshake_required(Initiator& ctx, const openpal::Timestamp& now) override;

            static Initiator::IHandshakeState* get()
            {
                static Idle instance;
                return &instance;
            }
        };

        class WaitForBeginReply final : public Initiator::IHandshakeState
        {
            WaitForBeginReply() {}

        public:

            static Initiator::IHandshakeState* get()
            {
                static WaitForBeginReply instance;
                return &instance;
            }
        };

        class WaitForAuthReply final : public Initiator::IHandshakeState
        {
            WaitForAuthReply() {}

        public:

            static Initiator::IHandshakeState* get()
            {
                static WaitForAuthReply instance;
                return &instance;
            }
        };

        class WaitForRetry final : public Initiator::IHandshakeState
        {
            WaitForRetry() {}

        public:

            static Initiator::IHandshakeState* get()
            {
                static WaitForRetry instance;
                return &instance;
            }
        };

        class BadConfiguration final : public Initiator::IHandshakeState
        {
            BadConfiguration() {}

        public:

            static Initiator::IHandshakeState* get()
            {
                static BadConfiguration instance;
                return &instance;
            }
        };
    };



}

#endif
