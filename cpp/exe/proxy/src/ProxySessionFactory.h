#ifndef SSP21PROXY_PROXYSESSIONFACTORY_H
#define SSP21PROXY_PROXYSESSIONFACTORY_H

#include "IProxySession.h"

#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>

#include <functional>

using proxy_session_factory_t = std::function<std::unique_ptr<IProxySession>(const log4cpp::Logger& logger, std::shared_ptr<exe4cpp::BasicExecutor> executor)>;

#endif
