#ifndef SSP21_MOCKUPPERLAYER_H
#define SSP21_MOCKUPPERLAYER_H

#include "openpal/util/Uncopyable.h"

#include "ssp21/LayerInterfaces.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include <deque>

namespace ssp21
{
    class MockUpperLayer : public IUpperLayer, private openpal::Uncopyable
    {

    public:

        void bind_lower(ILowerLayer& lower)
        {
            this->lower = &lower;
        }

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

        uint32_t num_tx_ready = 0;

    private:

        bool is_open = false;

        typedef std::deque<std::string> message_queue_t;

        message_queue_t rx_messages;

        ILowerLayer* lower = nullptr;

        virtual void on_open_impl() override
        {
            this->is_open = true;
        }

        virtual void on_close_impl() override
        {
            this->is_open = false;
        }

        virtual void on_tx_ready_impl() override
        {
            ++num_tx_ready;
        }

        virtual void start_rx_impl(const seq32_t& data) override
        {
            this->rx_messages.push_back(to_hex(data));

            // TODO - provide an option to differ this behavior
            this->lower->on_rx_ready();
        }

        virtual bool is_rx_ready_impl() override
        {
            return true;  // TODO
        }

    };

}

#endif
