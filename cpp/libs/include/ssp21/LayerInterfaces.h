
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
        *
        *   Start an asynchronous transmission. The underlying buffer pointed to by 'data'
        *   is loaned out to this layer and must not be mutated until IUpperLayer::on_tx_ready()
        *   is called.
        *
        *   @param data bytes to be transmitted
        */
        virtual bool start_tx(const seq32_t& data) = 0;

        /**
        *   True if ready to start_tx(..) will succeed
        */
        virtual bool is_tx_ready() const = 0;

        // tell this layer to push any data it might have
        virtual void receive() = 0;

    protected:

        void reset_lower_layer()
        {
            this->is_rx_ready = false;
        }

        // bool is_tx_ready = true;
        bool is_rx_ready = false;

    };

    class IUpperLayer
    {

    public:

        inline void on_open()
        {
            if (!this->is_open)
            {
                this->is_open = true;
                this->on_open_impl();
            }
        }

        inline void on_close()
        {
            if (this->is_open)
            {
                this->is_open = false;
                this->on_close_impl();
            }
        }

        inline void on_tx_ready()
        {
            if (this->is_open)
            {
                this->on_tx_ready_impl();
            }
        }

        inline bool on_rx_ready(const seq32_t& data)
        {
            return (this->is_open) ? this->on_rx_ready_impl(data) : false;
        }

        inline bool get_is_open() const
        {
            return is_open;
        }

    protected:

        virtual void on_open_impl() = 0;

        virtual void on_close_impl() = 0;

        virtual void on_tx_ready_impl() = 0;

        virtual bool on_rx_ready_impl(const seq32_t& data) = 0;

    private:

        bool is_open = false;

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