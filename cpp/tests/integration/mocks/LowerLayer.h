#ifndef SSP21_LOWERLAYER_H
#define SSP21_LOWERLAYER_H

#include "ssp21/LayerInterfaces.h"

#include "openpal/executor/IExecutor.h"
#include "openpal/container/Buffer.h"

#include <stdexcept>
#include <memory>
#include <deque>

#include "ssp21/MakeUnique.h"

namespace ssp21
{

    class LowerLayer final : public ILowerLayer
    {
        typedef openpal::Buffer message_t;

    public:

        explicit LowerLayer(const std::shared_ptr<openpal::IExecutor>& executor) : executor(executor)
        {}

        virtual bool start_tx(const seq32_t& data) override
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
                this->upper->on_tx_ready();
            });

            return true;
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

        virtual void on_rx_ready_impl() override
        {
            this->try_start_rx();
        }


        bool try_start_rx()
        {
            if (this->rx_processing || messages.empty()) return false;

            this->rx_processing = true;

            if (this->upper->start_rx(messages.front()->as_rslice()))
            {
                return true;
            }
            else
            {
                this->rx_processing = false;
                return false;
            }
        }

        // sibling layer notification that data has been placed in queue
        void on_new_data()
        {
            this->try_start_rx();
        }

        const std::shared_ptr<openpal::IExecutor> executor;

        std::deque<std::unique_ptr<message_t>> messages;

        // set during configure step
        IUpperLayer* upper = nullptr;
        LowerLayer* sibling = nullptr;
    };

}

#endif
