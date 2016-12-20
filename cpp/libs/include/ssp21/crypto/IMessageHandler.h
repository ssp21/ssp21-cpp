
#ifndef SSP21_IMESSAGE_HANDLER_H
#define SSP21_IMESSAGE_HANDLER_H

#include "ssp21/crypto/gen/RequestHandshakeAuth.h"
#include "ssp21/crypto/gen/RequestHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/ReplyHandshakeAuth.h"
#include "ssp21/crypto/gen/ReplyHandshakeError.h"
#include "ssp21/crypto/gen/UnconfirmedSessionData.h"

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

        virtual bool on_message(const UnconfirmedSessionData& msg, const openpal::RSlice& raw_data, const openpal::Timestamp& now)
        {
            return false;
        }

    };



}

#endif
