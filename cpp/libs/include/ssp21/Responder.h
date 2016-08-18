
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "ssp21/msg/RequestHandshakeBegin.h"
#include "ssp21/msg/RequestHandshakeAuth.h"
#include "ssp21/msg/UnconfirmedSessionData.h"
#include "ssp21/gen/Function.h"

#include "openpal/logging/Logger.h"

namespace ssp21
{
	class Responder
	{

	public:

		Responder(openpal::Logger logger);

		void on_rx_data(const openpal::RSlice& data);

	private:

		template <class MsgType>
		inline void read_any(const openpal::RSlice& data);

		/*
		
		*/		

		void on_message(const openpal::RSlice& data, const RequestHandshakeBegin& msg);		
		void on_message(const openpal::RSlice& data, const UnconfirmedSessionData& msg);
		void on_message(const openpal::RSlice& data, const RequestHandshakeAuth& msg);

		openpal::Logger logger_;
	};
}

#endif