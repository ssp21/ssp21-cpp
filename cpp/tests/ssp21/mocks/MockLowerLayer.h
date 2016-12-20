#ifndef SSP21_MOCKLOWERLAYER_H
#define SSP21_MOCKLOWERLAYER_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/LayerInterfaces.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include "MakeUnique.h"

namespace ssp21
{
    class MockLowerLayer : public ILowerLayer, private openpal::Uncopyable
    {

        typedef openpal::Buffer message_t;


    public:

        void set_tx_ready()
        {
            this->is_tx_ready = true;
        }

        virtual bool transmit(const openpal::RSlice& message) override
        {
            if (!this->is_tx_ready)
            {
                throw std::logic_error("transmit called when tx_ready == false");
            }

            this->tx_messages.push_back(std::make_unique<message_t>(message));
            this->is_tx_ready = false;
            return true;
        }

        virtual bool receive(IMessageProcessor& processor) override
        {
            if (this->rx_messages.empty())
            {
                return false;
            }

            auto& front = this->rx_messages.front();

            processor.process(front->as_rslice());

            this->rx_messages.pop_front();
            if (this->rx_messages.empty())
            {
                this->is_rx_ready = false;
            }

            return true;
        }

        void enqueue_message(const std::string& hex)
        {
            openpal::Hex hexdata(hex);
            this->rx_messages.push_back(std::make_unique<message_t>(hexdata.as_rslice()));
            this->is_rx_ready = true;
        }

        size_t num_rx_messages() const
        {
            return this->rx_messages.size();
        }

        std::string pop_tx_message()
        {
            if (this->tx_messages.empty())
            {
                throw std::logic_error("No messages to pop()");
            }

            auto hex = openpal::to_hex(this->tx_messages.front()->as_rslice());
            this->tx_messages.pop_front();
            return hex;
        }

    private:

        typedef std::deque<std::unique_ptr<message_t>> message_queue_t;

        message_queue_t tx_messages;

        message_queue_t rx_messages;
    };

}

#endif
