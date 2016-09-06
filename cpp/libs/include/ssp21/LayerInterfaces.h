
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
    *  A message between the link layer and the crypto layer
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
        virtual void process(const Message& message) = 0;
    };

    class ILowerLayer
    {

    public:

        virtual bool transmit(const Message& message) = 0;

        virtual bool receive(IMessageProcessor& processor) = 0;

        inline bool is_tx_ready() const
        {
            return is_tx_ready_;
        }

        inline bool is_rx_ready() const
        {
            return is_rx_ready_;
        }

    protected:

        bool is_tx_ready_ = true;
        bool is_rx_ready_ = false;

    };

    class IUpperLayer
    {

    public:

        inline void on_open()
        {
            if (!is_open_)
            {
                is_open_ = true;
                this->on_open_impl();
            }
        }

        inline void on_close()
        {
            if (is_open_)
            {
                is_open_ = false;
                this->on_close_impl();
            }
        }

        inline void on_tx_ready()
        {
            if (is_open_)
            {
                this->on_tx_ready_impl();
            }
        }

        inline void on_rx_ready()
        {
            if (is_open_)
            {
                this->on_rx_ready_impl();
            }
        }

    protected:

        virtual void on_open_impl() = 0;

        virtual void on_close_impl() = 0;

        virtual void on_tx_ready_impl() = 0;

        virtual void on_rx_ready_impl() = 0;

        inline bool is_open() const
        {
            return is_open_;
        }

    private:

        bool is_open_ = false;

    };

}

#endif