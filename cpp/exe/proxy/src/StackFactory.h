#ifndef SSP21PROXY_ISTACKFACTORY_H
#define SSP21PROXY_ISTACKFACTORY_H

#include <exe4cpp/IExecutor.h>
#include <log4cpp/Logger.h>
#include <ssp21/stack/IStack.h>
#include <yaml-cpp/yaml.h>

#include <functional>
#include <memory>

// abstracts the creation of responder or initiator
using stack_factory_t = std::function<std::shared_ptr<ssp21::IStack>(
    const log4cpp::Logger& logger,
    const std::shared_ptr<exe4cpp::IExecutor>& exe)>;

enum class StackType {
    initiator,
    responder
};

class StackFactory {
public:
    StackFactory(bool uses_link_layer, StackType type, stack_factory_t impl)
        : uses_link_layer(uses_link_layer)
        , type(type)
        , impl(impl)
    {
    }

    std::shared_ptr<ssp21::IStack> create_stack(const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& exe)
    {
        return impl(logger, exe);
    }

    StackType get_type() const
    {
        return this->type;
    }

    bool get_uses_link_layer() const
    {
        return this->uses_link_layer;
    }

private:
    bool uses_link_layer;
    StackType type;
    stack_factory_t impl;
};

#endif
