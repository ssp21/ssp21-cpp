
#include "ssp21/crypto/ResponderHandshakeStates.h"

namespace ssp21
{

HandshakeStateIdle HandshakeStateIdle::instance;
	
Responder::IHandshakeState& HandshakeStateIdle::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeBegin& msg)
{
	return *this;
}

Responder::IHandshakeState& HandshakeStateIdle::on_message(Responder::Context& ctx, const openpal::RSlice& msg_bytes, const RequestHandshakeAuth& msg)
{
	return *this;
}
	
      
}


