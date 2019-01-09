
#ifndef SSP21_IMESSAGE_HANDLER_H
#define SSP21_IMESSAGE_HANDLER_H

#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "crypto/gen/RequestHandshakeBegin.h"
#include "crypto/gen/ReplyHandshakeBegin.h"
#include "crypto/gen/ReplyHandshakeAuth.h"
#include "crypto/gen/ReplyHandshakeError.h"
#include "crypto/gen/SessionData.h"

#include "exe4cpp/Typedefs.h"

namespace ssp21
{

    /**
    * A handler for crypto messages
    */
    struct IMessageHandler
    {

        /**
        * determines if a function should be parsed
        */
        virtual bool supports(Function function) const = 0;

        /**
        * Called when a parse error occurs for a particular message type
        */
        virtual void on_parse_error(Function function, ParseError error) {}

        // --- optionally overriden message callbacks ---

        virtual bool on_message(const RequestHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
        {
            return false;
        }

        virtual bool on_message(const RequestHandshakeAuth& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
        {
            return false;
        }

        virtual bool on_message(const ReplyHandshakeBegin& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
        {
            return false;
        }

        virtual bool on_message(const ReplyHandshakeAuth& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
        {
            return false;
        }

        virtual bool on_message(const ReplyHandshakeError& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
        {
            return false;
        }

        virtual bool on_message(const SessionData& msg, const seq32_t& raw_data, const exe4cpp::steady_time_t& now)
        {
            return false;
        }

    };



}

#endif
