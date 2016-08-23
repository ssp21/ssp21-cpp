
#include "ssp21/crypto/Responder.h"

#include "openpal/logging/LogMacros.h"

#include "ssp21/LogLevels.h"
#include "ssp21/crypto/LogLinePrinter.h"

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
			FORMAT_LOG_BLOCK(logger_, levels::warn, "error reading %s: %s", FunctionSpec::to_string(MsgType::function), ParseErrorSpec::to_string(err));
		}
		else {			
			this->on_message(data, msg);
		}
	}

	void Responder::process_msg(const RSlice& data)
	{
		if (data.is_empty()) {
			SIMPLE_LOG_BLOCK(logger_, levels::warn, "Received zero length message");
			return;
		}

		const auto function = FunctionSpec::from_type(data[0]);
		
		switch (function)
		{
			case(Function::request_handshake_begin) :
				this->read_any<RequestHandshakeBegin>(data);
				break;
			case(Function::request_handshake_auth) :
				this->read_any<RequestHandshakeAuth>(data);
				break;
			
			case(Function::unconfirmed_session_data) :
				this->read_any<UnconfirmedSessionData>(data);
				break;
			
			default:
				FORMAT_LOG_BLOCK(logger_, levels::warn, "Received unknown function id: %u", data[0]);
				break;
		}
	}

	void Responder::on_message(const RSlice& data, const RequestHandshakeBegin& msg)
	{
		FORMAT_LOG_BLOCK(logger_, levels::rx_crypto_msg, "request handshake begin (length = %u)", data.length());

		if (logger_.is_enabled(levels::rx_crypto_msg_fields))
		{
			LogLinePrinter printer(logger_, levels::rx_crypto_msg_fields);
			msg.print(printer);
		}
	}

	void Responder::on_message(const RSlice& data, const RequestHandshakeAuth& msg)
	{
		FORMAT_LOG_BLOCK(logger_, levels::rx_crypto_msg, "request handshake auth (length = %u)", data.length());

		if (logger_.is_enabled(levels::rx_crypto_msg_fields))
		{
			LogLinePrinter printer(logger_, levels::rx_crypto_msg_fields);
			msg.print(printer);
		}
	}
	
	void Responder::on_message(const RSlice& data, const UnconfirmedSessionData& msg)
	{
		FORMAT_LOG_BLOCK(logger_, levels::rx_crypto_msg, "unconfirmed session data (length = %u)", data.length());

		if (logger_.is_enabled(levels::rx_crypto_msg_fields))
		{
			LogLinePrinter printer(logger_, levels::rx_crypto_msg_fields);
			msg.print(printer);
		}
	}

}
