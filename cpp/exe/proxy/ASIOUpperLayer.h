#ifndef SSP21PROXY_ASIOUPPERLAYER_H
#define SSP21PROXY_ASIOUPPERLAYER_H

#include <ssp21/stack/ILowerLayer.h>

#include "ASIOLayerBase.h"

class ASIOUpperLayer final : public ssp21::IUpperLayer, public ASIOLayerBase
{

public:

    ASIOUpperLayer(const openpal::Logger& logger, socket_t socket) : ASIOLayerBase(logger, std::move(socket))
    {}

    void bind(ssp21::ILowerLayer& crypto_layer, const std::function<void ()>& error_handler)
    {
        this->crypto_layer = &crypto_layer;
        this->error_handler = error_handler;
    }

private:

    // --- IUpperLayer ---

    virtual void on_open_from_lower_impl() override
    {
        this->start_rx_from_socket();
    }

    virtual void on_close_impl() override
    {
        this->try_close_socket();
        this->error_handler();
    }

    virtual void on_tx_ready_impl() override
    {
        this->crypto_layer->on_rx_ready();
    }

    virtual void start_rx_impl(const ssp21::seq32_t& data) override
    {
        this->start_tx_to_socket(data);
    }

    virtual bool is_rx_ready_impl() override
    {
        return !this->get_is_tx_active();
    }

    // --- ASIOLayerBase ---

    virtual void on_rx_complete(const ssp21::seq32_t& data) override
    {
        // when we receive socket data, try writing it to the crypto layer
        this->crypto_layer->start_tx(data);
    }

    virtual void on_tx_complete() override
    {
        // when we successfully transmit, tell the crypto layer we're ready for more data
        this->crypto_layer->on_rx_ready();
    }

    virtual void on_rx_or_tx_error() override
    {
        this->error_handler();
    }

    ssp21::ILowerLayer* crypto_layer = nullptr;
    std::function<void()> error_handler = nullptr;

};

#endif
