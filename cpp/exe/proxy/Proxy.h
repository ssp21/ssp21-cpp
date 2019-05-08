#ifndef SSP21PROXY_PROXY_H
#define SSP21PROXY_PROXY_H

#include <ser4cpp/util/Uncopyable.h>
#include <exe4cpp/asio/BasicExecutor.h>
#include <log4cpp/Logger.h>

#include <asio.hpp>

#include "ProxyConfig.h"
#include "StackFactory.h"
#include "Session.h"

#include <map>

class Proxy
{
public:
    virtual ~Proxy() = default;
    virtual void start() = 0;
};

#endif
