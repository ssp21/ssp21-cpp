#ifndef SSP21PROXY_ASIOLOWERLAYER_H
#define SSP21PROXY_ASIOLOWERLAYER_H

#include "ASIOLayerBase.h"

#include <ssp21/stack/ILowerLayer.h>

#include <asio.hpp>

class ASIOLowerLayer final : public ssp21::ILowerLayer, public ASIOLayerBase
{

public:

    ASIOLowerLayer(const openpal::Logger& logger, socket_t socket) : ASIOLayerBase(logger, std::move(socket))
    {}

    void open(ssp21::IUpperLayer& upper)
    {
        this->upper = &upper;
        this->start_rx_from_socket();
        this->upper->on_open();
    }

    bool close()
    {
        this->try_close_socket();

        if (this->upper->is_open())
        {
            return this->upper->on_close();
        }
        else
        {
            return false;
        }
    }

    // --- ILowerLayer ---

    virtual bool start_tx(const ssp21::seq32_t& data) override
    {
        return this->start_tx_to_socket(data);
    }

    virtual bool is_tx_ready() const override
    {
        return !this->get_is_tx_active();
    }

private:

    // --- ASIOLayerBase ---

    virtual void on_rx_complete(const ssp21::seq32_t& data) override
    {
        this->unread_data = data;
        this->try_start_upper_rx(this->unread_data);
    }

    virtual void on_rx_or_tx_error() override
    {
        this->upper->on_close();
    }

    virtual void on_tx_complete() override
    {
        this->upper->on_tx_ready();
    }

    // --- ILowerLayer ---

    virtual void discard_rx_data() override
    {
        this->unread_data.make_empty();
    }

    virtual void on_rx_ready_impl() override
    {
        if (this->unread_data.is_empty())
        {
            this->start_rx_from_socket();
        }
        else
        {
            this->try_start_upper_rx(this->unread_data);
        }
    }

    ssp21::seq32_t unread_data;
};

#endif
