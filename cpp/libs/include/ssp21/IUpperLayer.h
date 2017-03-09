
#ifndef SSP21_IUPPERLAYER_H
#define SSP21_IUPPERLAYER_H

#include "ssp21/util/SequenceTypes.h"

namespace ssp21
{
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
}

#endif