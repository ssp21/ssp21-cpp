
#ifndef SSP21_INITIATORHANDSHAKESTATES_H
#define SSP21_INITIATORHANDSHAKESTATES_H

#include "crypto/Initiator.h"

namespace ssp21 {
struct InitiatorHandshakeStates {
    class Idle final : public Initiator::IHandshakeState {
        Idle()
            : Initiator::IHandshakeState(Initiator::IHandshakeState::Enum::idle)
        {
        }

    public:
        virtual IHandshakeState* on_handshake_required(Initiator& ctx, const exe4cpp::steady_time_t& now) override;

        static Initiator::IHandshakeState* get()
        {
            static Idle instance;
            return &instance;
        }
    };

    class WaitForBeginReply final : public Initiator::IHandshakeState {
        WaitForBeginReply()
            : Initiator::IHandshakeState(Initiator::IHandshakeState::Enum::wait_for_begin_reply)
        {
        }

    public:
        virtual IHandshakeState* on_reply_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now) override;
        virtual IHandshakeState* on_error_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now) override;
        virtual IHandshakeState* on_response_timeout(Initiator& ctx) override;

        static Initiator::IHandshakeState* get()
        {
            static WaitForBeginReply instance;
            return &instance;
        }
    };

    class WaitForAuthReply final : public Initiator::IHandshakeState {
        WaitForAuthReply()
            : Initiator::IHandshakeState(Initiator::IHandshakeState::Enum::wait_for_auth_reply)
        {
        }

        virtual IHandshakeState* on_error_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now) override;

        virtual IHandshakeState* on_auth_message(Initiator& ctx, const SessionData& msg, const seq32_t& msg_bytes, const exe4cpp::steady_time_t& now) override;

        virtual IHandshakeState* on_response_timeout(Initiator& ctx) override;

    public:
        static Initiator::IHandshakeState* get()
        {
            static WaitForAuthReply instance;
            return &instance;
        }
    };

    class WaitForRetry final : public Initiator::IHandshakeState {
        WaitForRetry()
            : Initiator::IHandshakeState(Initiator::IHandshakeState::Enum::wait_for_retry)
        {
        }

        virtual IHandshakeState* on_retry_timeout(Initiator& ctx) override;

    public:
        static Initiator::IHandshakeState* get()
        {
            static WaitForRetry instance;
            return &instance;
        }
    };

    class BadConfiguration final : public Initiator::IHandshakeState {
        BadConfiguration()
            : Initiator::IHandshakeState(Initiator::IHandshakeState::Enum::bad_configuration)
        {
        }

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
