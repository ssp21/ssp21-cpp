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

        virtual bool transmit(const seq32_t& data) override
        {
            if (!this->is_tx_ready)
            {
                throw std::logic_error("already transmitting");
            }

            this->messages.push_back(std::make_unique<message_t>(data));

            this->is_tx_ready = false;

            // notify the sibling that theres data available to be read
            executor->post([sibling = this->sibling]()
            {
                sibling->on_sibling_rx_ready();
            });

            // simulate asynchronous transmission
            executor->post([this]()
            {
                this->is_tx_ready = true;
                this->upper->on_tx_ready();
            });

            return true;
        }

        // tell this layer to push any data it might have
        virtual void receive() override
        {
            sibling->read(*this->upper);
        }

        void configure(IUpperLayer& upper, LowerLayer& sibling)
        {
            this->upper = &upper;
            this->sibling = &sibling;
        }

    private:

        // sibling layer requests that the data be pushed into its upper layer
        bool read(IUpperLayer& upper)
        {
            if (messages.empty()) return false;

            if (upper.on_rx_ready(messages.front()->as_rslice()))
            {
                messages.pop_front();
                return true;
            }

            return false;
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
