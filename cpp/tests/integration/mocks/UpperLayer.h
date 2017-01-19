#ifndef SSP21_UPPERLAYER_H
#define SSP21_UPPERLAYER_H

#include "ssp21/LayerInterfaces.h"

namespace ssp21
{

    class UpperLayer final : public IUpperLayer
    {

    public:

        void configure(ILowerLayer& lower)
        {
            this->lower = &lower;
        }

    private:

        virtual void on_open_impl() {}

        virtual void on_close_impl() {}

        virtual void on_tx_ready_impl() {}

        virtual bool on_rx_ready_impl(const seq32_t& data)
        {
            return true;
        }

        // set during configure step
        ILowerLayer* lower = nullptr;
    };

}

#endif
