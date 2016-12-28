
#ifndef SSP21_IMESSAGE_HANDLER_H
#define SSP21_IMESSAGE_HANDLER_H

#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeAuth.h"
#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/gen/SessionData.h"

#include "openpal/executor/Timestamp.h"

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

        virtual bool on_message(const RequestHandshakeBegin& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
        {
            return false;
        }

        virtual bool on_message(const RequestHandshakeAuth& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
        {
            return false;
        }

        virtual bool on_message(const ReplyHandshakeBegin& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
        {
            return false;
        }

        virtual bool on_message(const ReplyHandshakeAuth& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
        {
            return false;
        }

        virtual bool on_message(const ReplyHandshakeError& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
        {
            return false;
        }

        virtual bool on_message(const SessionData& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
        {
            return false;
        }

    };



}

#endif
