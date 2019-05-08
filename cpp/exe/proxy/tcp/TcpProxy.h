#ifndef SSP21PROXY_TCPPROXY_H
#define SSP21PROXY_TCPPROXY_H

#include <ser4cpp/util/Uncopyable.h>
#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>

#include <asio.hpp>

#include "Proxy.h"
#include "ProxyConfig.h"
#include "StackFactory.h"
#include "Session.h"

#include <map>

/**
* A proxy has an accepting server and multiple concurrent sessions
*/
class TcpProxy final : public Proxy
{
    struct Server
    {
        Server(asio::io_service& context, const std::string& address, uint16_t port);

        asio::ip::tcp::endpoint endpoint;
        asio::ip::tcp::acceptor acceptor;
        asio::ip::tcp::socket socket;
        asio::ip::tcp::endpoint local_endpoint;
        asio::ip::tcp::endpoint remote_endpoint;
    };

    struct ConnectOperation
    {
        ConnectOperation(asio::io_service& context, asio::ip::tcp::socket socket) : connect_socket(context), listen_socket(std::move(socket))
        {}

        asio::ip::tcp::socket& get_lower_layer_socket(ProxyConfig::EndpointMode mode)
        {
            return (mode == ProxyConfig::EndpointMode::initiator) ? connect_socket : listen_socket;
        }

        asio::ip::tcp::socket& get_upper_layer_socket(ProxyConfig::EndpointMode mode)
        {
            return (mode == ProxyConfig::EndpointMode::initiator) ? listen_socket : connect_socket;
        }

        asio::ip::tcp::socket connect_socket;
        asio::ip::tcp::socket listen_socket;
    };

public:

    TcpProxy(
        const ProxyConfig& config,
        const std::shared_ptr<exe4cpp::BasicExecutor>& executor,
        const log4cpp::Logger& logger
    );

    void start() override;

private:

    void on_session_error(uint64_t session_id);

    std::map<uint64_t, std::shared_ptr<Session>> sessions;

    void accept_next();

    void start_connect(asio::ip::tcp::socket accepted_socket);

    const std::shared_ptr<exe4cpp::BasicExecutor> executor;
    log4cpp::Logger logger;
    stack_factory_t factory;

    Server server;
    asio::ip::tcp::endpoint connect_endpoint;
    const ProxyConfig::EndpointMode mode;
    const uint16_t max_sessions;

    uint64_t session_id = 0;
};

#endif
