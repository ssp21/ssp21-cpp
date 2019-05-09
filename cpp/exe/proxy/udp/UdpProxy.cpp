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
    listen_receive_endpoint(ip::address::from_string(config.listen_endpoint), config.listen_receive_port),
    listen_send_endpoint(ip::address::from_string(config.listen_endpoint), config.listen_send_port),
    destination_receive_endpoint(ip::address::from_string(config.destination_endpoint), config.destination_receive_port),
    destination_send_endpoint(ip::address::from_string(config.destination_endpoint), config.destination_send_port),
    mode(config.endpoint_mode),
    factory(config.factory)
{}

void UdpProxy::start()
{
    /*FORMAT_LOG_BLOCK(
        this->logger,
        levels::info,
        "listening for connections on %s:%u, forwarding to %s:%u",
        this->listen_endpoint.address().to_string().c_str(), this->listen_endpoint.port(),
        this->destination_endpoint.address().to_string().c_str(), this->destination_endpoint.port()
    );*/
    this->start_session();
}

void UdpProxy::on_session_error()
{
    // What do we do here, I don't know!
    // Shutdown and restart the session?
    //session->shutdown();
    session->shutdown();
    this->start_session();
}

void UdpProxy::start_session()
{
    auto error_handler = [this]()
    {
        this->on_session_error();
    };

    auto lower_receive_endpoint = mode == ProxyConfig::EndpointMode::initiator ? destination_receive_endpoint : listen_receive_endpoint;
    auto lower_send_endpoint = mode == ProxyConfig::EndpointMode::initiator ? destination_send_endpoint : listen_send_endpoint;
    auto lower_layer_logger = this->logger.detach_and_append("-lower");
    auto lower_layer = std::make_unique<AsioLowerLayer>(lower_layer_logger);
    auto lower_layer_socket = std::make_unique<AsioUdpSocketWrapper>(lower_layer_logger, *lower_layer, AsioUdpSocketWrapper::socket_t(*executor->get_service(), lower_receive_endpoint), lower_send_endpoint);

    auto upper_receive_endpoint = mode == ProxyConfig::EndpointMode::initiator ? listen_receive_endpoint : destination_receive_endpoint;
    auto upper_send_endpoint = mode == ProxyConfig::EndpointMode::initiator ? listen_send_endpoint : destination_send_endpoint;
    auto upper_layer_logger = this->logger.detach_and_append("-upper");
    auto upper_layer = std::make_unique<AsioUpperLayer>(upper_layer_logger);
    auto upper_layer_socket = std::make_unique<AsioUdpSocketWrapper>(upper_layer_logger, *upper_layer, AsioUdpSocketWrapper::socket_t(*executor->get_service(), upper_receive_endpoint), upper_send_endpoint);

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
