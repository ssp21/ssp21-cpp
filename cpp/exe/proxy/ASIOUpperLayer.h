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

    virtual void on_lower_open_impl() override
    {
        this->start_rx_from_socket();
    }

    virtual void on_lower_close_impl() override
    {
        this->try_close_socket();
        this->error_handler();
    }

    virtual void on_lower_tx_ready_impl() override
    {
        // read more data from the socket
        this->start_rx_from_socket();
    }

    virtual void on_lower_rx_ready_impl() override
    {
        // crypto layer has data to be read
        // try to read it if we're not already transmitting
        if (!this->get_is_tx_active()) this->try_read_from_crypto();
    }

    // --- ASIOLayerBase ---

    virtual void on_rx_complete(const ssp21::seq32_t& data) override
    {
        // when we receive socket data, try writing it to the crypto layer
        this->crypto_layer->start_tx_from_upper(data);
    }

    virtual void on_tx_complete() override
    {
        // when we successfully transmit to the socket,
        // try to read more data from the crypto layer
        this->try_read_from_crypto();
    }

    virtual void on_rx_or_tx_error() override
    {
        this->error_handler();
    }

    void try_read_from_crypto()
    {
        const auto data = this->crypto_layer->start_rx_from_upper();
        if (data.is_not_empty()) this->start_tx_to_socket(data);
    }

    ssp21::ILowerLayer* crypto_layer = nullptr;
    std::function<void()> error_handler = nullptr;

};

#endif
