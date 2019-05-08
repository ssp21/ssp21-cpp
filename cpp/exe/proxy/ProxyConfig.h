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
        EndpointMode endpoint_mode,
        uint16_t max_sessions,
        uint16_t listen_port,
        const std::string& listen_endpoint,
        uint16_t destination_port,
        const std::string& destination_endpoint
    ) :
        factory(factory),
        id(id),
        log_levels(log_levels),
        proto_type(proto_type),
        endpoint_mode(endpoint_mode),
        max_sessions(max_sessions),
        listen_port(listen_port),
        listen_endpoint(listen_endpoint),
        destination_port(destination_port),
        destination_endpoint(destination_endpoint)
    {}

    const stack_factory_t factory;
    const std::string id;

    const log4cpp::LogLevels log_levels;
    const ProtoType proto_type;
    const EndpointMode endpoint_mode;
    const uint16_t max_sessions;

    const uint16_t listen_port;
    const std::string listen_endpoint;

    const uint16_t destination_port;
    const std::string destination_endpoint;
};

#endif
