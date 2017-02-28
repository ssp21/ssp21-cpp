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
            this->messages.push_back(std::make_unique<message_t>(data));

            // notify the sibling that theres data available to be read
            executor->post([sibling = this->sibling]()
            {
                sibling->on_sibling_rx_ready();
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
            if (sibling->messages.empty()) throw std::logic_error("no sibling data to discard");
            else
            {
                sibling->messages.pop_front();
            }
        }

        virtual void on_rx_ready_impl() override
        {
            this->rx_processing = true;
            if (!sibling->read(*this->upper))
            {
                this->rx_processing = false;
            }
        }

        // sibling layer requests that the data be pushed into its upper layer
        bool read(IUpperLayer& upper)
        {
            return messages.empty() ? false : upper.start_rx(messages.front()->as_rslice());
        }

        void on_sibling_rx_ready()
        {
            sibling->read(*upper);
        }

        const std::shared_ptr<openpal::IExecutor> executor;

        std::deque<std::unique_ptr<message_t>> messages;

        // set during configure step
        IUpperLayer* upper = nullptr;
        LowerLayer* sibling = nullptr;
    };

}

#endif
