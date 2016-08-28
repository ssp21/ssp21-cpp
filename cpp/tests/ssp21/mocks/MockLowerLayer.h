#ifndef SSP21_MOCKLOWERLAYER_H
#define SSP21_MOCKLOWERLAYER_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/LayerInterfaces.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

namespace ssp21
{
    class MockLowerLayer : public ILowerLayer, private openpal::Uncopyable
    {

        typedef std::pair<Addresses, openpal::Buffer> message_t;


    public:

        virtual bool transmit(const Message& message) override
        {
            assert(this->is_tx_ready_);
            this->tx_messages_.push_back(
				std::unique_ptr<message_t>(new message_t(message.addresses, message.payload))
            );
            this->is_tx_ready_ = false;
			return true;
        }

        virtual bool receive(IMessageProcessor& processor) override
        {
            if (rx_messages_.empty())
            {
                return false;
            }

            auto& front = rx_messages_.front();

			processor.process(
				Message(front->first, front->second.as_rslice())
			);

            rx_messages_.pop_front();

            return true;
        }

        void enqueue_message(const Addresses& addr, const std::string& hex)
        {
            openpal::Hex hexdata(hex);
            this->rx_messages_.push_back(
                std::unique_ptr<message_t>(new message_t(addr, hexdata.as_rslice()))
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
                auto hex = openpal::to_hex(tx_messages_.front()->second.as_rslice());
                tx_messages_.pop_front();
                return hex;
            }
        }

    private:

        typedef std::deque<std::unique_ptr<message_t>> message_queue_t;

        message_queue_t tx_messages_;

        message_queue_t rx_messages_;
    };

}

#endif
