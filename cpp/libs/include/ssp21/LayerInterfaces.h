
#ifndef SSP21_LAYER_INTERFACES_H
#define SSP21_LAYER_INTERFACES_H

#include "openpal/container/RSlice.h"
#include "openpal/util/Uncopyable.h"

namespace ssp21
{
    /**
    * Addressing information from a source to a destination
    */
    struct Addresses
    {
        Addresses(uint16_t destination, uint16_t source) :
            destination(destination), source(source)
        {}

        Addresses() : destination(0), source(0)
        {}

        uint16_t destination;
        uint16_t source;
    };

    /**
    *  A combination of payload data and address information
    */
    struct Message : openpal::Uncopyable
    {
        Message(const Addresses& addresses, const openpal::RSlice& payload) :
            addresses(addresses),
            payload(payload)
        {

        }

        Message() {}

        Addresses addresses;
        openpal::RSlice payload;
    };

    class IMessageProcessor
    {
    public:
        virtual void process(const openpal::RSlice& data) = 0;
    };

    class ILowerLayer
    {

    public:

        virtual bool transmit(const openpal::RSlice& data) = 0;

        virtual bool receive(IMessageProcessor& processor) = 0;

        inline bool get_is_tx_ready() const
        {
            return this->is_tx_ready;
        }

        inline bool get_is_rx_ready() const
        {
            return this->is_rx_ready;
        }

    protected:

        bool is_tx_ready = true;
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

        inline void on_rx_ready()
        {
            if (this->is_open)
            {
                this->on_rx_ready_impl();
            }
        }

    protected:

        virtual void on_open_impl() = 0;

        virtual void on_close_impl() = 0;

        virtual void on_tx_ready_impl() = 0;

        virtual void on_rx_ready_impl() = 0;

        inline bool get_is_open() const
        {
            return is_open;
        }

    private:

        bool is_open = false;

    };

}

#endif