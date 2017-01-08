
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

            virtual IHandshakeState* on_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;
            virtual IHandshakeState* on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now) override;
            virtual IHandshakeState* on_response_timeout(Initiator& ctx) override;

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
