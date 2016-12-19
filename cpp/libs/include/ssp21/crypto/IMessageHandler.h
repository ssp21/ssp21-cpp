
#ifndef SSP21_IMESSAGE_HANDLER_H
#define SSP21_IMESSAGE_HANDLER_H

#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeAuth.h"
#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/gen/UnconfirmedSessionData.h"

namespace ssp21
{

    /**
    * A handler for crypto messages
    */
    struct IMessageHandler
    {
		// --- message callbacks ---

		virtual void on_message(const RequestHandshakeBegin& msg) = 0;
		
		virtual void on_message(const RequestHandshakeAuth& msg) = 0;
		
		virtual void on_message(const ReplyHandshakeBegin& msg) = 0;
		
		virtual void on_message(const ReplyHandshakeAuth& msg) = 0;
		
		virtual void on_message(const ReplyHandshakeError& msg) = 0;

		virtual void on_message(const UnconfirmedSessionData& msg) = 0;


		// --- error callbacks ---

		virtual void on_empty_message() = 0;

		virtual void on_unknown_function(uint8_t function) = 0;

		virtual void on_error(Function function, ParseError error) = 0;
		
    };



}

#endif
