#ifndef SSP21_MOCKUPPERLAYER_H
#define SSP21_MOCKUPPERLAYER_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/LayerInterfaces.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

namespace ssp21
{
    class MockUpperLayer : public IUpperLayer, private IMessageProcessor, private openpal::Uncopyable
    {

	public:
        
		

		MockUpperLayer(ILowerLayer& lower) : lower_(&lower)
		{}

		std::string pop_rx_message()
		{
			if (this->rx_messages_.empty())
			{
				return "";
			}
			else
			{
				auto hex = openpal::to_hex(rx_messages_.front()->second.as_rslice());
				rx_messages_.pop_front();
				return hex;
			}
		}

	private:

		typedef std::pair<Addresses, openpal::Buffer> message_t;

		typedef std::deque<std::unique_ptr<message_t>> message_queue_t;

		message_queue_t rx_messages_;

		ILowerLayer* lower_;

		virtual void process(const Message& message) override
		{
			rx_messages_.push_back(std::make_unique<message_t>(message.addresses, message.payload));
		}

		virtual void on_open_impl() override {}

		virtual void on_close_impl() override {}

		virtual void on_tx_ready_impl() override 
		{
		
		}

		virtual void on_rx_ready_impl() override 
		{
			lower_->receive(*this);
		}
		
    };

}

#endif
