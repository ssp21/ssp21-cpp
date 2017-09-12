#ifndef SSP21PROXY_STACKFACTORY_H
#define SSP21PROXY_STACKFACTORY_H

#include <openpal/logging/Logger.h>
#include <openpal/executor/IExecutor.h>

#include <functional>

#include <ssp21/stack/IStack.h>


// abstracts the creation of responder or initiator
typedef std::function<std::shared_ptr<ssp21::IStack>(
    const openpal::Logger& logger,
    const std::shared_ptr<openpal::IExecutor>& exe
)> stack_factory_t;

#endif
