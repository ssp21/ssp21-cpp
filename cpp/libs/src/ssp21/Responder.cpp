
#include "ssp21/Responder.h"

#include "openpal/logging/LogMacros.h"
#include "openpal/logging/LogLevels.h"

using namespace openpal;

namespace ssp21
{
	Responder::Responder(Logger logger) : logger_(logger)
	{
	
	}

	void Responder::on_rx_data(const RSlice& data)
	{
		if (data.is_empty()) {
			SIMPLE_LOG_BLOCK(logger_, logflags::WARN, "Received zero length message");
			return;
		}
		
		switch (FunctionSpec::from_type(data[0]))
		{
			case(Function::request_handshake_begin) :
				//RequestHandshakeBegin msg;
				
				break;
		
			case(Function::reply_handshake_auth) :
				break;
		
			case(Function::unconfirmed_session_data) :
				break;

			default:
				FORMAT_LOG_BLOCK(logger_, logflags::WARN, "Received unknown function id: %u", data[0]);
				break;
		}
	}

	void Responder::on_message(const RSlice& data, const RequestHandshakeBegin& msg)
	{
	
	}

	void Responder::on_message(const RequestHandshakeAuth& msg)
	{
	
	}
	
	void Responder::on_message(const RSlice& data, const UnconfirmedSessionData& msg)
	{
	
	}

}
