#ifndef SSP21PROXY_PROXYCONFIG_H
#define SSP21PROXY_PROXYCONFIG_H

#include "ser4cpp/util/Uncopyable.h"
#include "log4cpp/LogLevels.h"

#include "ssp21/crypto/BufferTypes.h"

#include "StackFactory.h"

#include <string>
#include <memory>

struct ProxyConfig : public ser4cpp::Uncopyable
{
    enum class ProtoType
    {
        tcp,
        udp,
    };

    enum class EndpointMode
    {
        initiator,
        responder,
    };

    ProxyConfig(
        const stack_factory_t& factory,
        const std::string& id,
        log4cpp::LogLevels log_levels,
        ProtoType proto_type,
        EndpointMode endpoint_mode
    ) :
        factory(factory),
        id(id),
        log_levels(log_levels),
        proto_type(proto_type),
        endpoint_mode(endpoint_mode)
    {}

    const stack_factory_t factory;
    const std::string id;

    const log4cpp::LogLevels log_levels;
    const ProtoType proto_type;
    const EndpointMode endpoint_mode;
};

#endif
