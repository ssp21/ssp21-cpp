
#include "ssp21/crypto/MessageDispatcher.h"

namespace ssp21
{
	bool MessageDispatcher::Dispatch(const openpal::RSlice& message, IMessageHandler& handler)
	{
		if (message.is_empty())
		{
			handler.on_empty_message();
			return false;
		}

		const auto function = message[0];

		switch (FunctionSpec::from_type(function))
		{
			case(Function::request_handshake_begin):
				return handle_message<RequestHandshakeBegin>(message, handler);
			case(Function::request_handshake_auth):
				return handle_message<RequestHandshakeAuth>(message, handler);
			case(Function::reply_handshake_begin):
				return handle_message<ReplyHandshakeBegin>(message, handler);
			case(Function::reply_handshake_auth):
				return handle_message<ReplyHandshakeAuth>(message, handler);
			case(Function::reply_handshake_error):
				return handle_message<ReplyHandshakeError>(message, handler);			
			case(Function::unconfirmed_session_data):				
				return handle_message<UnconfirmedSessionData>(message, handler);
			default:
				handler.on_unknown_function(function);
				return false;
		}
	}
}

