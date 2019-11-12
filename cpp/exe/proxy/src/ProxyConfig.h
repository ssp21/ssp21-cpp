#ifndef SSP21PROXY_PROXYCONFIG_H
#define SSP21PROXY_PROXYCONFIG_H

#include "ProxySessionFactory.h"

#include "ser4cpp/util/Uncopyable.h"

#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>

#include <vector>
#include <memory>

namespace config {

	std::vector<proxy_session_factory_t> read(const std::string& file_path, const std::shared_ptr<exe4cpp::BasicExecutor>& executor, const log4cpp::Logger& logger);

}

#endif
