
#include "ssp21/Responder.h"

#include "openpal/logging/LogMacros.h"
#include "openpal/logging/LogLevels.h"

using namespace openpal;

namespace ssp21
{
	Responder::Responder(Logger logger) : logger_(logger)
	{
	
	}

	template <class MsgType>
	inline void Responder::read_any(const openpal::RSlice& data)
	{
		MsgType msg;
		auto err = msg.read_msg(data);
		if (any(err)) {
			FORMAT_LOG_BLOCK(logger_, logflags::WARN, "error reading %s: %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));
		}
		else {
			this->on_message(data, msg);
		}
	}

	void Responder::on_rx_data(const RSlice& data)
	{
		if (data.is_empty()) {
			SIMPLE_LOG_BLOCK(logger_, logflags::WARN, "Received zero length message");
			return;
		}

		const auto function = FunctionSpec::from_type(data[0]);
		
		switch (function)
		{
			case(Function::request_handshake_begin) :
				this->read_any<RequestHandshakeBegin>(data);
				break;
			case(Function::reply_handshake_auth) :
				this->read_any<RequestHandshakeAuth>(data);
				break;
			
			case(Function::unconfirmed_session_data) :
				this->read_any<UnconfirmedSessionData>(data);
				break;
			
			default:
				FORMAT_LOG_BLOCK(logger_, logflags::WARN, "Received unknown function id: %u", data[0]);
				break;
		}
	}

	void Responder::on_message(const RSlice& data, const RequestHandshakeBegin& msg)
	{
	
	}

	void Responder::on_message(const RSlice& data, const RequestHandshakeAuth& msg)
	{
	
	}
	
	void Responder::on_message(const RSlice& data, const UnconfirmedSessionData& msg)
	{
	
	}

}
