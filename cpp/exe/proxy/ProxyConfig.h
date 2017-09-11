#ifndef SSP21PROXY_PROXYCONFIG_H
#define SSP21PROXY_PROXYCONFIG_H

#include "openpal/util/Uncopyable.h"
#include "openpal/logging/LogLevels.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/ICertificateHandler.h"
#include "ssp21/crypto/StaticKeys.h"

#include <string>
#include <memory>

struct ProxyConfig : public openpal::Uncopyable
{
    enum class EndpointMode
    {
        initiator,
        responder
    };

    enum class CertificateMode
    {
        preshared_keys,
        certificates
    };

    struct SSP21
    {
        SSP21(
            uint16_t local_address,
            uint16_t remote_address,
            const ssp21::StaticKeys& local_keys,
            const std::shared_ptr<ssp21::ICertificateHandler>& certificate_handler
        ) :
            local_address(local_address),
            remote_address(remote_address),
            local_keys(local_keys),
            certificate_handler(certificate_handler)
        {}

        SSP21() = delete;

        const uint16_t local_address;
        const uint16_t remote_address;

        const ssp21::StaticKeys local_keys;
        const std::shared_ptr<ssp21::ICertificateHandler> certificate_handler;
    };

    ProxyConfig(
        const std::string& id,
        openpal::LogLevels log_levels,
		EndpointMode mode,
        const SSP21& ssp21,
        uint16_t max_sessions,
        uint16_t listen_port,
        const std::string& listen_endpoint,
        uint16_t connect_port,
        const std::string& connect_endpoint
    ) :
        id(id),
        log_levels(log_levels),
        mode(mode),
        ssp21(ssp21),
        max_sessions(max_sessions),
        listen_port(listen_port),
        listen_endpoint(listen_endpoint),
        connect_port(connect_port),
        connect_endpoint(connect_endpoint)
    {}

    const std::string id;
    openpal::LogLevels log_levels;
    const EndpointMode mode;

    const SSP21 ssp21;

    const uint16_t max_sessions;

    const uint16_t listen_port;
    const std::string listen_endpoint;

    const uint16_t connect_port;
    const std::string connect_endpoint;
};

#endif
