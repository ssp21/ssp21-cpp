#ifndef SSP21PROXY_UDPPROXYSESSION_H
#define SSP21PROXY_UDPPROXYSESSION_H

#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>
#include <ser4cpp/util/Uncopyable.h>

#include <asio.hpp>

#include "IProxySession.h"
#include "Session.h"
#include "StackConfigReader.h"
#include "udp/AsioUdpSocketWrapper.h"
#include "udp/UdpConfig.h"

#include <memory>

class UdpProxySession final : public IProxySession {

public:
    UdpProxySession(
        const UdpConfig& config,
        const StackFactory& factory,
        const std::shared_ptr<exe4cpp::BasicExecutor>& executor,
        const log4cpp::Logger& logger);

    void start() override;

private:
    void on_session_error();

    void start_session();

    const std::shared_ptr<exe4cpp::BasicExecutor> executor;
    log4cpp::Logger logger;
    AsioUdpSocketWrapper::endpoint_t raw_tx_endpoint;
    AsioUdpSocketWrapper::endpoint_t raw_rx_endpoint;
    AsioUdpSocketWrapper::endpoint_t secure_tx_endpoint;
    AsioUdpSocketWrapper::endpoint_t secure_rx_endpoint;
    StackFactory factory;

    std::shared_ptr<Session> session;
};

#endif
