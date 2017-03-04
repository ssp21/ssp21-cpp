
#ifndef SSP21_LINKCRYPTOSTACK_H
#define SSP21_LINKCRYPTOSTACK_H

#include "ssp21/link/LinkLayer.h"
#include "ssp21/crypto/Responder.h"
#include "ssp21/link/LinkFrameWriter.h"

namespace ssp21
{
	class ResponderStack final : public IDualLayer
	{
	public:

		ResponderStack(
			Addresses addresses,
			const Session::Config& sconfig,
			const ResponderConfig& rconfig,
			openpal::Logger logger,
			const std::shared_ptr<openpal::IExecutor>& executor,
			const Keys& keys) :
				link(addresses.source, addresses.destination),
				responder(rconfig, sconfig, logger, get_frame_writer(logger, addresses, consts::link::max_config_payload_size), executor,  keys)
		{
			
		}

		virtual void bind(ILowerLayer& lower, IUpperLayer& upper) override
		{
			this->link.bind(lower, responder);
			this->responder.bind(link, upper);
		}

		virtual ILowerLayer& get_lower() override
		{
			return this->link;
		}

		virtual IUpperLayer& get_upper() override
		{
			return this->responder;
		}

	private:

		static std::shared_ptr<IFrameWriter> get_frame_writer(openpal::Logger logger, Addresses addresses, uint16_t max_payload_size)
		{
			return std::make_shared<LinkFrameWriter>(logger, addresses, max_payload_size);
		}

		LinkLayer link;
		Responder responder;
	};
    

}

#endif
