
#ifndef SSP21_MESSAGE_DISPATCHER_H
#define SSP21_MESSAGE_DISPATCHER_H


#include "IMessageHandler.h"

namespace ssp21
{

    /**
    * A handler for crypto messages
    */
    class MessageDispatcher
    {	
	public:
		static bool Dispatch(const openpal::RSlice& message, IMessageHandler& handler);

	private:

		template <class MsgType>
		static inline bool handle_message(const openpal::RSlice& message, IMessageHandler& handler)
		{
			MsgType msg;
			auto err = msg.read(message);
			if (any(err))
			{				
				handler.on_error(typename MsgType::function, err);
				return false;				
			}
			else
			{
				handler.on_message(msg);
				return true;
			}
		}

    };



}

#endif
