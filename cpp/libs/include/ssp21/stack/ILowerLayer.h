
#ifndef SSP21_ILOWERLAYER_H
#define SSP21_ILOWERLAYER_H

#include "IUpperLayer.h"

namespace ssp21
{
    /**
    * Performs asynchronous rx/tx operations on behalf of an IUpperLayer
    */
    class ILowerLayer
    {

    public:

        /**
        *
        *   Start an asynchronous tx operation. The underlying buffer pointed to by 'data'
        *   is loaned out to this layer and must not be mutated until IUpperLayer::on_tx_ready()
        *   is called.
        *
        *   @param data bytes to be transmitted
        */
        virtual bool start_tx(const seq32_t& data) = 0;

        /**
        *   Check the transmit readiness of the layer
        *
        *   @return true if ready to start_tx(..) will succeed, false if it will fail
        */
        virtual bool is_tx_ready() const = 0;

        /**
        *    Called by the upper layer when its ready to receive the next chunk of data via IUpperLayer::start_rx(...)
        *    Any previously received slices are no longer valid, and the underlying buffers made be mutated for the
        *    next start_rx(..) operation.
        */
        inline void on_rx_ready()
        {
            if (this->rx_processing)
            {
                this->discard_rx_data();
                this->rx_processing = false;
            }

            this->on_rx_ready_impl();
        }

    protected:

        IUpperLayer* upper = nullptr;

        /**
        * Called when a previous Upper->start_rx() operation completes
        */
        virtual void discard_rx_data() = 0;

        /**
        * Called to check if any data is available for reading
        */
        virtual void on_rx_ready_impl() = 0;

        inline bool try_start_upper_rx(const seq32_t& data)
        {
            this->rx_processing = true;
            const auto started = upper->start_rx(data);
            if (!started)
            {
                this->rx_processing = false;
            }
            return started;
        }

        inline void reset_this_lower_layer()
        {
            this->rx_processing = false;
        }

        inline bool is_rx_processing() const
        {
            return rx_processing;
        }

    private:

        /**
        * Tracks if the upper layer is currently processing some data
        */
        bool rx_processing = false;

    };
}

#endif
