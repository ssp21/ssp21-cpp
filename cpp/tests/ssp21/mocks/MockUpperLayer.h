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

        MockUpperLayer(ILowerLayer& lower) : lower(&lower)
        {}

        std::string pop_rx_message()
        {
            if (this->rx_messages.empty())
            {
                throw std::logic_error("No rx messages");
            }

            auto hex = this->rx_messages.front();
            this->rx_messages.pop_front();
            return hex;
        }

        bool is_empty() const
        {
            return this->rx_messages.empty();
        }

        bool get_is_open() const
        {
            return this->is_open;
        }

    private:

        bool is_open = false;

        typedef openpal::Buffer message_t;

        typedef std::deque<std::string> message_queue_t;

        message_queue_t rx_messages;

        ILowerLayer* const lower;

        virtual void process(const seq32_t& message) override
        {
            this->rx_messages.push_back(openpal::to_hex(message));
        }

        virtual void on_open_impl() override
        {
            this->is_open = true;
        }

        virtual void on_close_impl() override
        {
            this->is_open = false;
        }

        virtual void on_tx_ready_impl() override {}

        virtual void on_rx_ready_impl() override
        {
            this->lower->receive(*this);
        }

    };

}

#endif
