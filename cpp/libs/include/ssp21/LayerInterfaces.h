
#ifndef SSP21_LAYER_INTERFACES_H
#define SSP21_LAYER_INTERFACES_H

#include "ssp21/SequenceTypes.h"

namespace ssp21
{
    /**
    * Performs asynchronous rx/tx operations on behalf of an IUpperLayer
    */
    class ILowerLayer
    {

    public:

        /**
        * tracks the state of upper layer data processing
        */
        enum class RxState
        {
            /// upper layer has not requested data
            idle,
            /// upper layer has requested data, but is not currently processing any
            ready,
            /// upper layer is currently processing data
            processing
        };

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
        void on_rx_ready()
        {
            if (this->rx_processing)
            {
                this->discard_rx_data();
                this->rx_processing = false;
            }

            this->on_rx_ready_impl();
        }

    protected:

        virtual void discard_rx_data() = 0;

        virtual void on_rx_ready_impl() = 0;

        bool rx_processing = false;

    };

    /**
    * Performs asynchronous rx/tx operations on behalf of an ILowerLayer
    */
    class IUpperLayer
    {

    public:

        /**
        * open the layer if closed
        *
        * @return false if the layer is already open, true otherwise
        */
        inline bool on_open()
        {
            if (this->is_open_flag)
            {
                return false;
            }
            else
            {
                this->is_open_flag = true;
                this->on_open_impl();
                return true;
            }
        }

        /**
        * close the layer if closed
        *
        * @return false if the layer is already closed, true otherwise
        */
        inline bool on_close()
        {
            if (this->is_open_flag)
            {
                this->is_open_flag = false;
                this->on_close_impl();
                return true;
            }
            else
            {
                return false;
            }
        }

        /**
        *   Start an asynchronous rx operation. The underlying buffer pointed to by 'data'
        *   is loaned out to this layer and must not be mutated until ILowerLayer::on_rx_ready()
        *   is called.
        *
        *   @return true if the operation has been started, false otherwise.
        */
        inline bool start_rx(const seq32_t& data)
        {
            if (this->is_rx_ready())
            {
                this->start_rx_impl(data);
                return true;
            }
            else
            {
                return false;
            }
        }

        /**
        *   Check the transmit readiness of the layer
        *
        *   @return true if ready to start_rx(..) will succeed, false if it will fail
        */
        inline bool is_rx_ready()
        {
            return this->is_open_flag && this->is_rx_ready_impl();
        }

        /**
        * Called by the lower layer when the ready to transmit on behalf of the upper layer
        *
        * @return true if the layer is open, false if closed
        */
        inline bool on_tx_ready()
        {
            if (this->is_open_flag)
            {
                this->on_tx_ready_impl();
                return true;
            }
            else
            {
                return false;
            }
        }

        /**
        * @return true if the layer is open, false if closed.
        */
        inline bool is_open() const
        {
            return is_open_flag;
        }

    protected:

        virtual void on_open_impl() = 0;

        virtual void on_close_impl() = 0;

        virtual void on_tx_ready_impl() = 0;

        virtual void start_rx_impl(const seq32_t& data) = 0;

        virtual bool is_rx_ready_impl() = 0;

    private:

        bool is_open_flag = false;

    };

    /**
     * dual layers are both lower/upper and can be bound to a lower/upper
     */
    class IDualLayer : public ILowerLayer, public IUpperLayer
    {
    public:

        virtual ~IDualLayer() {}

        virtual void bind(ILowerLayer& lower, IUpperLayer& upper) = 0;
    };

}

#endif