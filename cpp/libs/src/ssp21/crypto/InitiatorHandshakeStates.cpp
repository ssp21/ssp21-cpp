
#include "ssp21/crypto/InitiatorHandshakeStates.h"

namespace ssp21
{
    Initiator::IHandshakeState& InitiatorHandshakeStateIdle::initialize(Initiator& ctx, const openpal::Timestamp& now)
    {
        return *this;
    }

    Initiator::IHandshakeState& InitiatorHandshakeStateIdle::on_message(Initiator& ctx, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        return *this;
    }

    Initiator::IHandshakeState& InitiatorHandshakeStateIdle::on_message(Initiator& ctx, const ReplyHandshakeAuth& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        return *this;
    }

    Initiator::IHandshakeState& InitiatorHandshakeStateIdle::on_message(Initiator& ctx, const ReplyHandshakeError& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now)
    {
        return *this;
    }

    Initiator::IHandshakeState& InitiatorHandshakeStateIdle::on_response_timeout(Initiator& ctx)
    {
        return *this;
    }
}


