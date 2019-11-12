#ifndef SSP21PROXY_ISTACKFACTORY_H
#define SSP21PROXY_ISTACKFACTORY_H

#include <log4cpp/Logger.h>
#include <exe4cpp/IExecutor.h>
#include <ssp21/stack/IStack.h>
#include <yaml-cpp/yaml.h>

#include <functional>
#include <memory>

// abstracts the creation of responder or initiator
using stack_factory_t = std::function<std::shared_ptr<ssp21::IStack>(
    const log4cpp::Logger& logger,
    const std::shared_ptr<exe4cpp::IExecutor>& exe
)>;

enum class StackType {
    initiator,
    responder
};

class StackFactory
{
public:
    StackFactory(StackType type, stack_factory_t impl) : type(type), impl(impl)
    {}

    std::shared_ptr<ssp21::IStack> create_stack(const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& exe)
    {
        return impl(logger, exe);
    }

    StackType get_type() const
    {
        return this->type;
    }

private:
    StackType type;
    stack_factory_t impl;
};

#endif

