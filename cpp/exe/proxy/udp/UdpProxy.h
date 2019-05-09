#ifndef SSP21PROXY_UDPPROXY_H
#define SSP21PROXY_UDPPROXY_H

#include <ser4cpp/util/Uncopyable.h>
#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>

#include <asio.hpp>

#include "Proxy.h"
#include "ProxyConfig.h"
#include "StackFactory.h"
#include "Session.h"
#include "udp/AsioUdpSocketWrapper.h"

#include <memory>

class UdpProxy final : public Proxy
{

public:

    UdpProxy(
        const ProxyConfig& config,
        const std::shared_ptr<exe4cpp::BasicExecutor>& executor,
        const log4cpp::Logger& logger
    );

    void start() override;

private:

    void on_session_error();

    void start_session();

    const std::shared_ptr<exe4cpp::BasicExecutor> executor;
    log4cpp::Logger logger;
    AsioUdpSocketWrapper::endpoint_t listen_endpoint;
    AsioUdpSocketWrapper::endpoint_t destination_endpoint;
    ProxyConfig::EndpointMode mode;
    stack_factory_t factory;

    std::shared_ptr<Session> session;
};

#endif
