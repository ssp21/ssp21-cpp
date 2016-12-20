
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

        static void handle_parse_error(openpal::Logger& logger, Function function, ParseError error, IMessageHandler& handler);

        template <class MsgType>
        static inline bool handle_message(openpal::Logger& logger, const openpal::RSlice& message, const openpal::Timestamp& now, IMessageHandler& handler)
        {
            MsgType msg;
            auto err = msg.read(message);
            if (any(err))
            {
                handle_parse_error(logger, MsgType::function, err, handler);
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
