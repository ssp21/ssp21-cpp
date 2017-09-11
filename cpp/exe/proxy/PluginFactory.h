#ifndef SSP21PROXY_PLUGINFACTORY_H
#define SSP21PROXY_PLUGINFACTORY_H

#include <openpal/util/Uncopyable.h>
#include <openpal/logging/Logger.h>
#include <openpal/executor/IExecutor.h>

#include <functional>

#include <ssp21/stack/IStack.h>

#include "ProxyConfig.h"


// abstracts the creation of responder or initiator
typedef std::function<std::shared_ptr<ssp21::IStack>(
    const openpal::Logger& logger,
    const std::shared_ptr<openpal::IExecutor>& exe
)> plugin_factory_t;


class PluginFactory : private openpal::StaticOnly
{

public:

    static plugin_factory_t get(const ProxyConfig& config);


};


#endif
