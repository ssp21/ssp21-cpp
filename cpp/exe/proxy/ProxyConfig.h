#ifndef SSP21PROXY_PROXYCONFIG_H
#define SSP21PROXY_PROXYCONFIG_H

#include <string>
#include <memory>

#include <openpal/util/Uncopyable.h>
#include <ssp21/crypto/BufferTypes.h>

struct ProxyConfig : public openpal::Uncopyable
{
    enum class Mode
    {
        initiator,
        responder
    };

    struct SSP21
    {
        SSP21(
            uint16_t local_address,
            uint16_t remote_address,
            const std::shared_ptr<const ssp21::PublicKey>& local_public_key,
            const std::shared_ptr<const ssp21::PrivateKey>& local_private_key,
            const std::shared_ptr<const ssp21::PublicKey>& remote_public_key
        ) :
            local_address(local_address),
            remote_address(remote_address),
            local_public_key(local_public_key),
            local_private_key(local_private_key),
            remote_public_key(remote_public_key)
        {}

        SSP21() = delete;

        const uint16_t local_address;
        const uint16_t remote_address;

        const std::shared_ptr<const ssp21::PublicKey> local_public_key;
        const std::shared_ptr<const ssp21::PrivateKey> local_private_key;
        const std::shared_ptr<const ssp21::PublicKey> remote_public_key;
    };

    ProxyConfig(
        const std::string& id,
        Mode mode,
        const SSP21& ssp21,
        uint16_t max_sessions,
        uint16_t listen_port,
        const std::string& listen_endpoint,
        uint16_t connect_port,
        const std::string& connect_endpoint
    ) :
        id(id),
        mode(mode),
        ssp21(ssp21),
        max_sessions(max_sessions),
        listen_port(listen_port),
        listen_endpoint(listen_endpoint),
        connect_port(connect_port),
        connect_endpoint(connect_endpoint)
    {}

    const std::string id;
    const Mode mode;

    const SSP21 ssp21;

    const uint16_t max_sessions;

    const uint16_t listen_port;
    const std::string listen_endpoint;

    const uint16_t connect_port;
    const std::string connect_endpoint;
};

#endif
