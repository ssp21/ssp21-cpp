#ifndef SSP21PROXY_PROXYCONFIG_H
#define SSP21PROXY_PROXYCONFIG_H

#include "openpal/util/Uncopyable.h"
#include "openpal/logging/LogLevels.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/link/Addresses.h"

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

    ProxyConfig(
        const std::string& id,
        openpal::LogLevels log_levels,
		EndpointMode endpoint_mode,
		CertificateMode cert_mode,
		ssp21::Addresses addresses,
        uint16_t max_sessions,
        uint16_t listen_port,
        const std::string& listen_endpoint,
        uint16_t connect_port,
        const std::string& connect_endpoint
    ) :
        id(id),
        log_levels(log_levels),
		endpoint_mode(endpoint_mode),
		addresses(addresses),
		cert_mode(cert_mode),
        max_sessions(max_sessions),
        listen_port(listen_port),
        listen_endpoint(listen_endpoint),
        connect_port(connect_port),
        connect_endpoint(connect_endpoint)
    {}

    const std::string id;

    const openpal::LogLevels log_levels;
    const EndpointMode endpoint_mode;
	const CertificateMode cert_mode;
	const ssp21::Addresses addresses;

    const uint16_t max_sessions;

    const uint16_t listen_port;
    const std::string listen_endpoint;

    const uint16_t connect_port;
    const std::string connect_endpoint;
};

#endif
