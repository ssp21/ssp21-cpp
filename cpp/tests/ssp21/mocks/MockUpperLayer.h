#ifndef SSP21_MOCKUPPERLAYER_H
#define SSP21_MOCKUPPERLAYER_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/LayerInterfaces.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include "MakeUnique.h"

namespace ssp21
{
    class MockUpperLayer : public IUpperLayer, private IMessageProcessor, private openpal::Uncopyable
    {

    public:

        MockUpperLayer(ILowerLayer& lower) : lower(&lower)
        {}

        std::string pop_rx_message()
        {
            if (this->rx_messages.empty())
            {
                return "";
            }
            else
            {
                auto hex = openpal::to_hex(this->rx_messages.front()->as_rslice());
				this->rx_messages.pop_front();
                return hex;
            }
        }

    private:

        typedef openpal::Buffer message_t;

        typedef std::deque<std::unique_ptr<message_t>> message_queue_t;

        message_queue_t rx_messages;

        ILowerLayer* const lower;

        virtual void process(const openpal::RSlice& message) override
        {
            this->rx_messages.push_back(std::make_unique<message_t>(message));
        }

        virtual void on_open_impl() override {}

        virtual void on_close_impl() override {}

        virtual void on_tx_ready_impl() override {}

        virtual void on_rx_ready_impl() override
        {
            this->lower->receive(*this);
        }

    };

}

#endif
