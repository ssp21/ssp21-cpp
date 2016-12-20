
#ifndef SSP21_MESSAGE_DISPATCHER_H
#define SSP21_MESSAGE_DISPATCHER_H


#include "IMessageHandler.h"

#include "openpal/logging/Logger.h"

namespace ssp21
{

    /**
    * A handler for crypto messages
    */
    class MessageDispatcher
    {
    public:

        static bool Dispatch(openpal::Logger& logger, const openpal::RSlice& message, const openpal::Timestamp& now, IMessageHandler& handler);

    private:

        static void log_parse_error(openpal::Logger& logger, Function function, ParseError);

        template <class MsgType>
        static inline bool handle_message(openpal::Logger& logger, const openpal::RSlice& message, const openpal::Timestamp& now, IMessageHandler& handler)
        {
            MsgType msg;
            auto err = msg.read(message);
            if (any(err))
            {
                log_parse_error(logger, MsgType::function, err);
                return false;
            }
            else
            {
                handler.on_message(msg, message, now);
                return true;
            }
        }

    };



}

#endif
