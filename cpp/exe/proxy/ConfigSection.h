#ifndef SSP21PROXY_CONFIGSECTION_H
#define SSP21PROXY_CONFIGSECTION_H

#include "ProxyConfig.h"
#include "tcp/TcpProxyConfig.h"
#include "udp/UdpProxyConfig.h"

#include "ssp21/crypto/gen/ContainerEntryType.h"
#include "ssp21/crypto/gen/HandshakeMode.h"
#include "ssp21/util/SecureDynamicBuffer.h"
#include "ssp21/stack/IStack.h"
#include "ssp21/link/Addresses.h"
#include "ssp21/crypto/StaticKeys.h"

#include "StackFactory.h"

#include <map>
#include <memory>

class QIXKeyCache;

class ConfigSection : ser4cpp::Uncopyable
{

public:

    void add(const std::string& propertyId, const std::string& value);

    std::unique_ptr<ProxyConfig> get_config(const log4cpp::Logger& logger, const std::string& id);

private:

    std::map<std::string, std::string> values;

    std::unique_ptr<TcpProxyConfig> get_tcp_config(const log4cpp::Logger& logger, const std::string& id);

    std::unique_ptr<UdpProxyConfig> get_udp_config(const log4cpp::Logger& logger, const std::string& id);

    stack_factory_t get_stack_factory(const log4cpp::Logger& logger, ProxyConfig::EndpointMode ep_mode, bool crypto_only = false);

    stack_factory_t get_initiator_factory(const log4cpp::Logger& logger, const ssp21::Addresses* addresses);

    stack_factory_t get_initiator_shared_secret_factory(const ssp21::Addresses* addresses);

    stack_factory_t get_initiator_qkd_factory(const log4cpp::Logger& logger, const ssp21::Addresses* addresses);

    stack_factory_t get_initiator_preshared_public_key_factory(const ssp21::Addresses* addresses);

    stack_factory_t get_initiator_certificate_mode_factory(const ssp21::Addresses* addresses);

    stack_factory_t get_responder_factory(const log4cpp::Logger& logger, const ssp21::Addresses* addresses);

    stack_factory_t get_responder_shared_secret_factory(const ssp21::Addresses* addresses);

    stack_factory_t get_responder_qkd_factory(const log4cpp::Logger& logger, const ssp21::Addresses* addresses);

    stack_factory_t get_responder_preshared_public_key_factory(const ssp21::Addresses* addresses);

    stack_factory_t get_responder_certificate_mode_factory(const ssp21::Addresses* addresses);

    ssp21::Addresses get_addresses();

    ssp21::StaticKeys get_local_static_keys();

    std::shared_ptr<const ssp21::SymmetricKey> get_shared_secret();

    log4cpp::LogLevels get_levels();

    std::string consume_value(const std::string& propertyId);

    std::shared_ptr<QIXKeyCache> get_qix_key_cache(const log4cpp::Logger& logger);

    template <typename T>
    T get_integer_value(const std::string& propertyId);

    bool get_boolean_value(const std::string& propertyId);

    ProxyConfig::ProtoType get_proto_type();

    ProxyConfig::EndpointMode get_mode();

    ssp21::HandshakeMode get_handshake_mode();

    std::shared_ptr<ssp21::SecureDynamicBuffer> get_file_data(const std::string& property);

    template <class T>
    std::shared_ptr<const T> get_crypto_key(const std::string& key, ssp21::ContainerEntryType expectedType);

    static log4cpp::LogLevels get_levels_for_char(char value);

};

#endif

