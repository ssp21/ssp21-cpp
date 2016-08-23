
#ifndef SSP21_RESPONDER_H
#define SSP21_RESPONDER_H

#include "ssp21/msg/RequestHandshakeBegin.h"
#include "ssp21/msg/RequestHandshakeAuth.h"
#include "ssp21/msg/UnconfirmedSessionData.h"

#include "ssp21/gen/Function.h"

#include "ssp21/LayerInterfaces.h"

#include "openpal/logging/Logger.h"
#include "openpal/container/Buffer.h"

#include "ssp21/link/LinkConstants.h"

namespace ssp21
{
	/**
		WIP - this class will implement the stateful part of the responder.
	*/
	class Responder final : public IUpperLayer
	{

	public:

		struct Config
		{
			uint16_t max_tx_message_size = consts::max_config_link_payload_size;
		};

		Responder(const Config& config, openpal::Logger logger, ILowerLayer& lower);

		// ---- implement IUpperLayer -----

		virtual void on_open_impl() override;
		virtual void on_close_impl() override;
		virtual void on_tx_ready_impl() override;
		virtual void on_rx_ready_impl() override;
		
	private:

		void process_msg(const openpal::RSlice& data);

		template <class MsgType>
		inline void read_any(const openpal::RSlice& data);

		void on_message(const openpal::RSlice& data, const RequestHandshakeBegin& msg);		
		void on_message(const openpal::RSlice& data, const UnconfirmedSessionData& msg);
		void on_message(const openpal::RSlice& data, const RequestHandshakeAuth& msg);

		Config config_;
		openpal::Logger logger_;
		ILowerLayer* lower_;
		openpal::Buffer tx_buffer_;

	};
}

#endif