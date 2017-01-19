#ifndef SSP21_LOWERLAYER_H
#define SSP21_LOWERLAYER_H

#include "ssp21/LayerInterfaces.h"

#include <openpal/executor/IExecutor.h>

#include <stdexcept>
#include <memory>

namespace ssp21
{

    class LowerLayer final : public ILowerLayer
    {

    public:

        explicit LowerLayer(const std::shared_ptr<openpal::IExecutor>& executor) : executor(executor)
        {}

        virtual bool transmit(const seq32_t& data) override
        {
            if (this->tx_data.is_not_empty())
            {
                throw std::logic_error("already transmitting");
            }

            this->tx_data = data;

            this->is_tx_ready = false;

            executor->post([sibling = this->sibling]()
            {
                sibling->on_sibling_rx_ready();
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
            if (tx_data.is_empty()) return false;

            if (upper.on_rx_ready(tx_data))
            {
                tx_data.make_empty();

                this->is_tx_ready = true;

                executor->post([upper = this->upper]()
                {
                    upper->on_tx_ready();
                });

                return true;
            }

            return false;
        }

        void on_sibling_rx_ready()
        {
            sibling->read(*upper);
        }

        const std::shared_ptr<openpal::IExecutor> executor;

        seq32_t tx_data;

        // set during configure step
        IUpperLayer* upper = nullptr;
        LowerLayer* sibling = nullptr;
    };

}

#endif
