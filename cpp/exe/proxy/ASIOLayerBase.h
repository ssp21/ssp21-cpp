#ifndef SSP21PROXY_ASIOLAYERBASE_H
#define SSP21PROXY_ASIOLAYERBASE_H

#include <openpal/util/Uncopyable.h>
#include <openpal/container/Buffer.h>
#include <openpal/logging/Logger.h>

#include <ssp21/link/LinkConstants.h>
#include <ssp21/util/SequenceTypes.h>
#include <ssp21/stack/LogLevels.h>

#include <openpal/logging/LogMacros.h>

#include <asio.hpp>

class ASIOLayerBase : private openpal::Uncopyable
{

public:

    typedef asio::ip::tcp::socket socket_t;

    ASIOLayerBase(const openpal::Logger& logger, socket_t socket) :
        socket(std::move(socket)),
        logger(logger),
        rx_buffer(ssp21::consts::link::max_frame_size)
    {}

    inline bool is_active() const
    {
        return this->is_rx_active || this->is_tx_active;
    }

protected:

    inline bool try_close_socket()
    {
        if (!this->socket.is_open()) return false;

        std::error_code ec;
        this->socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        this->socket.close(ec);

        return true;
    }

    virtual void on_rx_complete(const ssp21::seq32_t& data) = 0;
    virtual void on_tx_complete() = 0;
    virtual void on_rx_or_tx_error() = 0;


    bool start_rx_from_socket()
    {
        if (!this->socket.is_open() || this->is_rx_active) return false;

        auto callback = [this](const std::error_code & ec, size_t num_rx)
        {
            this->is_rx_active = false;

            if (ec)
            {
                if (socket.is_open())
                {
                    FORMAT_LOG_BLOCK(this->logger, ssp21::levels::error, "rx error: %s", ec.message().c_str());
                    this->try_close_socket();
                    this->on_rx_or_tx_error();
                }
            }
            else
            {
                const auto rx_data = this->rx_buffer.as_rslice().take<uint32_t>(num_rx);
                FORMAT_LOG_BLOCK(this->logger, ssp21::levels::debug, "complete socket rx: %u", rx_data.length());
                openpal::HexLogging::log(this->logger, ssp21::levels::debug, rx_data);
                this->on_rx_complete(rx_data);
            }
        };

        auto dest = rx_buffer.as_wslice();

        this->is_rx_active = true;

        SIMPLE_LOG_BLOCK(this->logger, ssp21::levels::debug, "start socket rx");

        this->socket.async_read_some(asio::buffer(dest, dest.length()), callback);

        return true;
    }

    bool start_tx_to_socket(const ssp21::seq32_t& data)
    {
        if (!this->socket.is_open() || this->is_tx_active) return false;

        auto callback = [this](const std::error_code & ec, size_t num_tx)
        {
            this->is_tx_active = false;

            if (ec)
            {
                if (socket.is_open())
                {
                    FORMAT_LOG_BLOCK(this->logger, ssp21::levels::error, "tx error: %s", ec.message().c_str());
                    this->try_close_socket();
                    this->on_rx_or_tx_error();
                }
            }
            else
            {
                FORMAT_LOG_BLOCK(this->logger, ssp21::levels::debug, "complete socket tx: %u", static_cast<uint32_t>(num_tx));
                this->on_tx_complete();
            }
        };

        this->is_rx_active = true;

        FORMAT_LOG_BLOCK(this->logger, ssp21::levels::debug, "start socket tx: %d", data.length());

        asio::async_write(this->socket, asio::buffer(data, data.length()), callback);

        return true;
    }

    inline bool get_is_tx_active() const
    {
        return this->is_tx_active;
    }

    inline bool get_is_rx_active() const
    {
        return this->is_rx_active;
    }

private:

    bool is_tx_active = false;
    bool is_rx_active = false;

    socket_t socket;

protected:

    openpal::Logger logger;
    openpal::Buffer rx_buffer;
};

#endif
