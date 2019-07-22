#include "udp/UdpProxy.h"

#include <log4cpp/LogMacros.h>
#include <ssp21/stack/LogLevels.h>

#include "Session.h"
#include "AsioLowerLayer.h"
#include "AsioUpperLayer.h"

using namespace asio;
using namespace ssp21;

UdpProxy::UdpProxy(
    const UdpProxyConfig& config,
    const std::shared_ptr<exe4cpp::BasicExecutor>& executor,
    const log4cpp::Logger& logger
) :
    executor(executor),
    logger(logger),
    raw_tx_endpoint(ip::address::from_string(config.raw_tx_endpoint.ip_address), config.raw_tx_endpoint.port),
    raw_rx_endpoint(ip::address::from_string(config.raw_rx_endpoint.ip_address), config.raw_rx_endpoint.port),
    secure_tx_endpoint(ip::address::from_string(config.secure_tx_endpoint.ip_address), config.secure_tx_endpoint.port),
    secure_rx_endpoint(ip::address::from_string(config.secure_rx_endpoint.ip_address), config.secure_rx_endpoint.port),
    mode(config.endpoint_mode),
    factory(config.factory)
{}

void UdpProxy::start()
{
    FORMAT_LOG_BLOCK(
        this->logger,
        levels::info,
        "receiving raw traffic on %s:%u, wrapping in SSP21, and sending to %s:%u",
        this->raw_rx_endpoint.address().to_string().c_str(), this->raw_rx_endpoint.port(),
        this->secure_tx_endpoint.address().to_string().c_str(), this->secure_tx_endpoint.port()
    );

    FORMAT_LOG_BLOCK(
        this->logger,
        levels::info,
        "receiving SSP21 traffic on %s:%u, unwrapping, and sending raw traffic to %s:%u",
        this->secure_rx_endpoint.address().to_string().c_str(), this->secure_rx_endpoint.port(),
        this->raw_tx_endpoint.address().to_string().c_str(), this->raw_tx_endpoint.port()
    );

    this->start_session();
}

void UdpProxy::on_session_error()
{
    session->shutdown();

    // Restart the session
    this->start_session();
}

void UdpProxy::start_session()
{
    auto error_handler = [this]()
    {
        this->on_session_error();
    };

    AsioUdpSocketWrapper::endpoint_t lower_receive_endpoint(this->secure_rx_endpoint);
    AsioUdpSocketWrapper::endpoint_t lower_send_endpoint(this->secure_tx_endpoint);
    AsioUdpSocketWrapper::endpoint_t upper_receive_endpoint(this->raw_rx_endpoint);
    AsioUdpSocketWrapper::endpoint_t upper_send_endpoint(this->raw_tx_endpoint);    

    auto lower_layer_logger = this->logger.detach_and_append("-lower");
    auto lower_layer = std::make_unique<AsioLowerLayer>(lower_layer_logger);
    auto lower_layer_socket = std::make_unique<AsioUdpSocketWrapper>(
        lower_layer_logger,
        *lower_layer,
        AsioUdpSocketWrapper::socket_t(
            *executor->get_service(),
            lower_receive_endpoint
        ),
        lower_send_endpoint
    );

    auto upper_layer_logger = this->logger.detach_and_append("-upper");
    auto upper_layer = std::make_unique<AsioUpperLayer>(upper_layer_logger);
    auto upper_layer_socket = std::make_unique<AsioUdpSocketWrapper>(
        upper_layer_logger,
        *upper_layer,
        AsioUdpSocketWrapper::socket_t(
            *executor->get_service(),
            upper_receive_endpoint
        ),
        upper_send_endpoint
    );

    this->session = Session::create(
                                0,
                                error_handler,
                                this->executor,
                                std::move(lower_layer_socket),
                                std::move(lower_layer),
                                std::move(upper_layer_socket),
                                std::move(upper_layer),
                                factory(
                                    this->logger.detach_and_append("-ssp21"),
                                    this->executor
                                )
                            );

    this->session->start();
}
