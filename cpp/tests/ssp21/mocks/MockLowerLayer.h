#ifndef SSP21_MOCKLOWERLAYER_H
#define SSP21_MOCKLOWERLAYER_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/LayerInterfaces.h"

#include "openpal/container/Buffer.h"

#include "testlib/BufferHelpers.h"
#include "testlib/HexConversions.h"

namespace ssp21
{
	class MockLowerLayer : public ILowerLayer, private openpal::Uncopyable
	{
		struct MessagePair
		{
			MessagePair(Addresses addresses, const openpal::RSlice& message) :
				addresses(addresses),
				buffer(message)
			{}

			Addresses addresses;
			openpal::Buffer buffer;
		};


	public:

		virtual void begin_transmit(const Addresses& addr, const openpal::RSlice& message)
		{
			assert(!this->is_transmitting_);
			this->tx_messages_.push_back(
				std::make_unique<MessagePair>(addr, message)
			);
			this->is_transmitting_ = true;
		}

		virtual bool read_message(IMessageConsumer& consumer)
		{
			if (rx_messages_.empty()) 
			{
				return false;
			}

			auto& front = rx_messages_.front();
			consumer.consume_message(front->addresses, front->buffer.as_rslice());			
			rx_messages_.pop_front();
			
			return true;
		}

		void enqueue_message(const Addresses& addr, const std::string& hex)
		{	
			openpal::Hex hexdata(hex);
			this->rx_messages_.push_back(
				std::make_unique<MessagePair>(addr, hexdata)
			);
		}

		std::string pop_tx_message()
		{
			if (this->tx_messages_.empty())
			{
				return "";
			}
			else
			{ 
				auto hex = openpal::to_hex(tx_messages_.front()->buffer.as_rslice());
				tx_messages_.pop_front();
				return hex;
			}
		}
		
	private:

		std::deque<std::unique_ptr<MessagePair>> tx_messages_;

		std::unique_ptr<MessagePair> current_rx_message_;

		std::deque<std::unique_ptr<MessagePair>> rx_messages_;
	};

}

#endif