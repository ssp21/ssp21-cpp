#ifndef SSP21PROXY_STACKFACTORY_H
#define SSP21PROXY_STACKFACTORY_H

#include <log4cpp/Logger.h>
#include <exe4cpp/IExecutor.h>

#include <functional>

#include <ssp21/stack/IStack.h>

// abstracts the creation of responder or initiator
using stack_factory_t = std::function<std::shared_ptr<ssp21::IStack>(
    const log4cpp::Logger& logger,
    const std::shared_ptr<exe4cpp::IExecutor>& exe
)>;

#endif
