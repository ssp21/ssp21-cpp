#ifndef SSP21PROXY_ASIOTCPSOCKETWRAPPER_H
#define SSP21PROXY_ASIOTCPSOCKETWRAPPER_H

#include <log4cpp/LogMacros.h>
#include <log4cpp/Logger.h>
#include <ser4cpp/container/Buffer.h>
#include <ser4cpp/util/Uncopyable.h>

#include <ssp21/link/LinkConstants.h>
#include <ssp21/stack/LogLevels.h>
#include <ssp21/util/SequenceTypes.h>

#include "IAsioLayer.h"
#include "IAsioSocketWrapper.h"

#include <asio.hpp>

class AsioTcpSocketWrapper final : public IAsioSocketWrapper, private ser4cpp::Uncopyable {

public:
    using socket_t = asio::ip::tcp::socket;

    AsioTcpSocketWrapper(const log4cpp::Logger& logger, IAsioLayer& layer, socket_t& socket)
        : layer(layer)
        , socket(std::move(socket))
        , logger(logger)
        , rx_buffer(ssp21::consts::link::max_frame_size)
    {
    }

    bool try_close_socket() override
    {
        if (!this->socket.is_open())
            return false;

        std::error_code ec;
        this->socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        this->socket.close(ec);

        return true;
    }

    bool start_rx_from_socket() override
    {
        if (!this->socket.is_open() || this->is_rx_active)
            return false;

        auto callback = [this](const std::error_code& ec, size_t num_rx) {
            this->is_rx_active = false;

            if (ec) {
                if (socket.is_open()) {
                    FORMAT_LOG_BLOCK(this->logger, ssp21::levels::error, "rx error: %s", ec.message().c_str());
                    this->try_close_socket();
                    this->layer.on_rx_or_tx_error();
                }
            } else {
                const auto rx_data = this->rx_buffer.as_rslice().take(static_cast<uint32_t>(num_rx));
                FORMAT_LOG_BLOCK(this->logger, ssp21::levels::debug, "complete socket rx: %u, tx: %s", rx_data.length(), bool_str(this->is_tx_active));
                if (this->logger.is_enabled(ssp21::levels::debug)) {
                    log4cpp::HexLogging::log(this->logger, ssp21::levels::debug, rx_data);
                }
                this->layer.on_rx_complete(rx_data);
            }
        };

        auto dest = rx_buffer.as_wslice();

        this->is_rx_active = true;

        FORMAT_LOG_BLOCK(this->logger, ssp21::levels::debug, "start socket rx, tx: %s", bool_str(this->is_tx_active));

        this->socket.async_read_some(asio::buffer(dest, dest.length()), callback);

        return true;
    }

    bool start_tx_to_socket(const ssp21::seq32_t& data) override
    {
        if (!this->socket.is_open() || this->is_tx_active)
            return false;

        auto callback = [this](const std::error_code& ec, size_t num_tx) {
            this->is_tx_active = false;

            if (ec) {
                if (socket.is_open()) {
                    FORMAT_LOG_BLOCK(this->logger, ssp21::levels::error, "tx error: %s", ec.message().c_str());
                    this->try_close_socket();
                    this->layer.on_rx_or_tx_error();
                }
            } else {
                FORMAT_LOG_BLOCK(this->logger, ssp21::levels::debug, "complete socket tx: %u - rx: %s", static_cast<uint32_t>(num_tx), bool_str(this->is_rx_active));
                this->layer.on_tx_complete();
            }
        };

        this->is_tx_active = true;

        FORMAT_LOG_BLOCK(this->logger, ssp21::levels::debug, "start socket tx: %d, rx: %s", data.length(), bool_str(this->is_tx_active));

        asio::async_write(this->socket, asio::buffer(data, data.length()), callback);

        return true;
    }

    bool get_is_tx_active() const override
    {
        return this->is_tx_active;
    }

    bool get_is_rx_active() const override
    {
        return this->is_rx_active;
    }

private:
    static const char* bool_str(bool value)
    {
        return value ? "true" : "false";
    }

    bool is_tx_active = false;
    bool is_rx_active = false;

    IAsioLayer& layer;
    socket_t socket;
    log4cpp::Logger logger;
    ser4cpp::Buffer rx_buffer;
};

#endif
