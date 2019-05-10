#ifndef SSP21PROXY_UDPPROXY_H
#define SSP21PROXY_UDPPROXY_H

#include <ser4cpp/util/Uncopyable.h>
#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>

#include <asio.hpp>

#include "Proxy.h"
#include "udp/UdpProxyConfig.h"
#include "StackFactory.h"
#include "Session.h"
#include "udp/AsioUdpSocketWrapper.h"

#include <memory>

class UdpProxy final : public Proxy
{

public:

    UdpProxy(
        const UdpProxyConfig& config,
        const std::shared_ptr<exe4cpp::BasicExecutor>& executor,
        const log4cpp::Logger& logger
    );

    void start() override;

private:

    void on_session_error();

    void start_session();

    const std::shared_ptr<exe4cpp::BasicExecutor> executor;
    log4cpp::Logger logger;
    AsioUdpSocketWrapper::endpoint_t source_receive_endpoint;
    AsioUdpSocketWrapper::endpoint_t source_send_endpoint;
    AsioUdpSocketWrapper::endpoint_t destination_receive_endpoint;
    AsioUdpSocketWrapper::endpoint_t destination_send_endpoint;
    ProxyConfig::EndpointMode mode;
    stack_factory_t factory;

    std::shared_ptr<Session> session;
};

#endif
