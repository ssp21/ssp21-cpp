#include "Proxy.h"

#include <openpal/logging/LogMacros.h>

#include "Session.h"

using namespace openpal;
using namespace asio;

Proxy::Server::Server(asio::io_service& context, const std::string& address, uint16_t port) :
    acceptor(context),
    socket(context),
    local_endpoint(ip::address::from_string(address), port)
{
    acceptor.open(this->local_endpoint.protocol());
    acceptor.bind(this->local_endpoint);
    acceptor.listen();
}

Proxy::Proxy(
    const ProxyConfig& config,
    const std::shared_ptr<Executor>& executor,
    const openpal::Logger& logger
) :
    executor(executor),
    logger(logger),
    factory(PluginFactory::get(config)),
    server(executor->get_context(), config.listen_endpoint, config.listen_port),
    connect_endpoint(ip::address::from_string(config.connect_endpoint), config.connect_port),
    mode(config.mode),
    max_sessions(config.max_sessions == 0 ? 1 : config.max_sessions)
{}

void Proxy::start()
{
    FORMAT_LOG_BLOCK(
        this->logger,
        levels::info,
        "listening for connections on %s:%u, forwarding to %s:%u",
        this->server.local_endpoint.address().to_string().c_str(), this->server.local_endpoint.port(),
        this->connect_endpoint.address().to_string().c_str(), this->connect_endpoint.port()
    );
    this->accept_next();
}

void Proxy::on_session_error(uint64_t session_id)
{
    const auto iter = this->sessions.find(session_id);
    if (iter != this->sessions.end())
    {
        const auto session = iter->second;
        this->sessions.erase(iter);
        session->shutdown();
    }
}

void Proxy::accept_next()
{
    auto accept_callback = [this](std::error_code ec)
    {
        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, levels::info, "accept failure: %s", ec.message().c_str());
        }
        else
        {
            FORMAT_LOG_BLOCK(this->logger, levels::info, "Accepted connection from %s:%u", this->server.remote_endpoint.address().to_string().c_str(), this->server.remote_endpoint.port());

            this->start_connect(std::move(this->server.socket));

            this->accept_next();
        }
    };

    server.acceptor.async_accept(server.socket, server.remote_endpoint, accept_callback);
}

void Proxy::start_connect(asio::ip::tcp::socket accepted_socket)
{
    // let's now kick off a connect operation
    // won't need this once C++XX has move capture
    const auto connect = std::make_shared<ConnectOperation>(executor->get_context(), std::move(accepted_socket));

    auto connect_cb = [this, connect](const std::error_code & ec)
    {
        if (ec)
        {
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "error connecting: %s", ec.message().c_str());
            std::error_code ec;
            connect->listen_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
            connect->listen_socket.close(ec);
        }
        else
        {
            FORMAT_LOG_BLOCK(this->logger, levels::warn, "connected to %s:%u", connect_endpoint.address().to_string().c_str(), connect_endpoint.port());

            if (this->sessions.size() == this->max_sessions) // have to kick a session to make room for new session
            {
                // max_sessions guaranteed to be > 0
                const auto oldest = this->sessions.begin();
                FORMAT_LOG_BLOCK(this->logger, levels::warn, "Max sessions exceeded. Shutting down oldest session: %lud", oldest->first);
                oldest->second->shutdown();
                this->sessions.erase(oldest);
            }

            const auto id = this->session_id++;

            // this will get called when any error occurs on the session
            auto error_handler = [this, id]()
            {
                this->on_session_error(id);
            };

            const auto session = Session::create(
                                     id,
                                     error_handler,
                                     this->executor,
                                     std::make_unique<socket_lower_layer_t>(
                                         this->logger.detach_and_append("-", id, "-lower"),
                                         std::move(connect->get_lower_layer_socket(this->mode))
                                     ),
                                     std::make_unique<socket_upper_layer_t>(
                                         this->logger.detach_and_append("-", id, "-upper"),
                                         std::move(connect->get_upper_layer_socket(this->mode))
                                     ),
                                     factory(
                                         this->logger.detach_and_append("-", id, "-ssp21"),
                                         this->executor
                                     )
                                 );

            this->sessions[id] = session;

            session->start();
        }
    };

    FORMAT_LOG_BLOCK(this->logger, levels::info, "Initiating connection to %s:%u", connect_endpoint.address().to_string().c_str(), connect_endpoint.port());
    connect->connect_socket.async_connect(this->connect_endpoint, connect_cb);
}
