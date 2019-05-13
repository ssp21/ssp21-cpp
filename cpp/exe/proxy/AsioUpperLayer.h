#ifndef SSP21PROXY_ASIOUPPERLAYER_H
#define SSP21PROXY_ASIOUPPERLAYER_H

#include <ssp21/stack/ILowerLayer.h>

#include "IAsioLayer.h"

#include <functional>

class AsioUpperLayer final : public ssp21::IUpperLayer, public IAsioLayer
{

public:

    AsioUpperLayer(const log4cpp::Logger& logger)
    {}

    void bind(IAsioSocketWrapper& socket, ssp21::ILowerLayer& crypto_layer, const std::function<void ()>& error_handler)
    {
        this->socket = &socket;
        this->crypto_layer = &crypto_layer;
        this->error_handler = error_handler;
    }

private:

    void try_read_from_crypto()
    {
        const auto data = this->crypto_layer->start_rx_from_upper();
        if (data.is_not_empty()) this->socket->start_tx_to_socket(data);
    }

    // --- IUpperLayer ---

    void on_lower_open_impl() override
    {
        this->socket->start_rx_from_socket();
    }

    void on_lower_close_impl() override
    {
        this->socket->try_close_socket();
        this->error_handler();
    }

    void on_lower_tx_ready_impl() override
    {
        // read more data from the socket
        this->socket->start_rx_from_socket();
    }

    void on_lower_rx_ready_impl() override
    {
        // crypto layer has data to be read
        // try to read it if we're not already transmitting
        if (!this->socket->get_is_tx_active()) this->try_read_from_crypto();
    }

public:

    // --- IAsioLayer ---

    void on_rx_complete(const ssp21::seq32_t& data) override
    {
        // when we receive socket data, try writing it to the crypto layer
        this->crypto_layer->start_tx_from_upper(data);
    }

    void on_tx_complete() override
    {
        // when we successfully transmit to the socket,
        // try to read more data from the crypto layer
        this->try_read_from_crypto();
    }

    void on_rx_or_tx_error() override
    {
        this->error_handler();
    }

    bool is_active() const override
    {
        return this->socket->is_active();
    }

private:

    ssp21::ILowerLayer* crypto_layer = nullptr;
    IAsioSocketWrapper* socket = nullptr;
    std::function<void()> error_handler = nullptr;

};

#endif
