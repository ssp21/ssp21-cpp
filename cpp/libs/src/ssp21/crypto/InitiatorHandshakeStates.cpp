
#include "ssp21/crypto/InitiatorHandshakeStates.h"

namespace ssp21
{

    Initiator::IHandshakeState* InitiatorHandshakeIdle::on_handshake_required(Initiator& ctx, const openpal::Timestamp& now)
    {
        // TODO - kick off the handshake

        return this;
    }
}


