#ifndef SSP21_LOWERLAYER_H
#define SSP21_LOWERLAYER_H

#include "ssp21/stack/ILowerLayer.h"
#include "ssp21/stack/IUpperLayer.h"

#include "exe4cpp/IExecutor.h"
#include "ser4cpp/container/Buffer.h"

#include <stdexcept>
#include <memory>
#include <deque>

namespace ssp21
{

    class LowerLayer final : public ILowerLayer
    {
        typedef ser4cpp::Buffer message_t;

    public:

        explicit LowerLayer(const std::shared_ptr<exe4cpp::IExecutor>& executor) : executor(executor)
        {}

        virtual bool start_tx_from_upper(const seq32_t& data) override
        {
            this->sibling->messages.push_back(std::make_unique<message_t>(data));

            // notify the sibling that there's data available to be read
            executor->post([this]()
            {
                this->sibling->on_new_data();
            });

            // simulate asynchronous transmission
            executor->post([this]()
            {
                this->upper->on_lower_tx_ready();
            });

            return true;
        }

        virtual seq32_t start_rx_from_upper_impl() override
        {
            return this->messages.empty() ? seq32_t::empty() : messages.front()->as_rslice();
        }

        virtual bool is_tx_ready() const override
        {
            return true;
        }

        void configure(IUpperLayer& upper, LowerLayer& sibling)
        {
            this->upper = &upper;
            this->sibling = &sibling;
        }

    private:

        virtual void discard_rx_data() override
        {
            if (this->messages.empty())
            {
                throw std::logic_error("no messages to discard");
            }
            else
            {
                this->messages.pop_front();
            }
        }

        // sibling layer notification that data has been placed in queue
        void on_new_data()
        {
            this->upper->on_lower_rx_ready();
        }

        const std::shared_ptr<exe4cpp::IExecutor> executor;

        std::deque<std::unique_ptr<message_t>> messages;

        // set during configure step
        LowerLayer* sibling = nullptr;
        IUpperLayer* upper = nullptr;
    };

}

#endif
