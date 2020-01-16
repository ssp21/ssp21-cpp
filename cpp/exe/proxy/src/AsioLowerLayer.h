#ifndef SSP21PROXY_ASIOLOWERLAYER_H
#define SSP21PROXY_ASIOLOWERLAYER_H

#include "IAsioLayer.h"
#include "IAsioSocketWrapper.h"

#include <ssp21/stack/ILowerLayer.h>
#include <ssp21/stack/IUpperLayer.h>

#include <log4cpp/LogMacros.h>

#include <asio.hpp>

class AsioLowerLayer final : public ssp21::ILowerLayer, public IAsioLayer {

public:
    AsioLowerLayer(const log4cpp::Logger& logger)
    {
    }

    void open(IAsioSocketWrapper& socket, ssp21::IUpperLayer& upper)
    {
        this->socket = &socket;
        this->upper = &upper;

        this->socket->start_rx_from_socket();
        this->upper->on_lower_open();
    }

    bool close()
    {
        this->socket->try_close_socket();
        return this->upper->on_lower_close();
    }

private:
    // --- ILowerLayer ---

    bool start_tx_from_upper(const ssp21::seq32_t& data) override
    {
        return this->socket->start_tx_to_socket(data);
    }

    bool is_tx_ready() const override
    {
        return !this->socket->get_is_tx_active();
    }

    void discard_rx_data() override
    {
        this->unread_data.make_empty();
    }

    ssp21::seq32_t start_rx_from_upper_impl() override
    {
        if (this->unread_data.is_empty())
            this->socket->start_rx_from_socket();

        return this->unread_data;
    }

public:
    // --- IAsioLayer ---

    void on_rx_complete(const ssp21::seq32_t& data) override
    {
        this->unread_data = data;
        this->upper->on_lower_rx_ready();
    }

    void on_rx_or_tx_error() override
    {
        this->upper->on_lower_close();
    }

    void on_tx_complete() override
    {
        this->upper->on_lower_tx_ready();
    }

    bool is_active() const override
    {
        return this->socket->is_active();
    }

private:
    IAsioSocketWrapper* socket = nullptr;
    ssp21::IUpperLayer* upper = nullptr;
    ssp21::seq32_t unread_data;
};

#endif
