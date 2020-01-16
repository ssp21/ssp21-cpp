
#include "ConfigReader.h"

#include "ssp21/link/Addresses.h"
#include "ssp21/util/SecureFile.h"

#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "ssp21/crypto/gen/ContainerFile.h"
#include "ssp21/crypto/gen/HandshakeMode.h"
#include "ssp21/crypto/gen/ParseError.h"

#include "ssp21/stack/Factory.h"
#include "ssp21/stack/LogLevels.h"

#include "ssp21/util/Exception.h"
#include "ssp21/util/SecureFile.h"

#include "StackConfigReader.h"
#include "YAMLHelpers.h"
#include "qkd/QKDSourceRegistry.h"

using namespace ssp21;

namespace config {
Addresses get_addresses(const YAML::Node& node)
{
    return Addresses(
        yaml::require_integer<uint16_t>(node, "remote"),
        yaml::require_integer<uint16_t>(node, "local"));
}

StackType get_stack_type(const YAML::Node& node)
{
    const auto value = yaml::require_string(node, "mode");

    if (value == "initiator") {
        return StackType::initiator;
    } else if (value == "responder") {
        return StackType::responder;
    } else {
        throw yaml::YAMLException(node, "Unknown SSP21 mode: ", value);
    }
}

ssp21::HandshakeMode get_handshake_mode(const YAML::Node& node)
{
    const auto value = yaml::require_string(node, "type");

    if (value == "shared_secret") {
        return HandshakeMode::shared_secret;
    } else if (value == "qkd") {
        return HandshakeMode::quantum_key_distribution;
    } else if (value == "preshared_public_key") {
        return HandshakeMode::preshared_public_keys;
    } else if (value == "certificate") {
        return HandshakeMode::industrial_certificates;
    } else {
        throw yaml::YAMLException(node.Mark(), "Unknown handshake mode: ", value);
    }
}

template <class T>
std::shared_ptr<const T> get_crypto_key(const std::string& path, ssp21::ContainerEntryType expected_type)
{
    const auto file_data = SecureFile::read(path);

    ContainerFile file;
    const auto err = file.read_all(file_data->as_rslice());
    if (any(err)) {
        throw Exception("Error (", ParseErrorSpec::to_string(err), ")  reading container file: ", path);
    }

    if (file.container_entry_type != expected_type) {
        throw Exception("Unexpected file type (", ContainerEntryTypeSpec::to_string(file.container_entry_type), ") in file: ", path);
    }

    const auto expected_length = ssp21::BufferBase::get_buffer_length(ssp21::BufferType::x25519_key);
    if (file.payload.length() != expected_length) {
        throw Exception("Unexpected key length: ", file.payload.length());
    }

    const auto ret = std::make_shared<T>();

    ret->as_wseq().copy_from(file.payload);
    ret->set_type(BufferType::x25519_key);

    return ret;
}

std::shared_ptr<ssp21::SecureDynamicBuffer> get_file_data(const YAML::Node& node, const std::string& key)
{
    return ssp21::SecureFile::read(yaml::require_string(node, key));
}

std::shared_ptr<const SymmetricKey> get_shared_secret(const YAML::Node& node)
{
    return get_crypto_key<SymmetricKey>(yaml::require_string(node, "shared_secret_key_path"), ContainerEntryType::shared_secret);
}

uint32_t get_optional_ms_from_duration(const YAML::Node& node, const std::string& key, uint32_t default_value)
{
    const auto duration = yaml::optional_duration(node, key, std::chrono::milliseconds(default_value));
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    if (ms < 0) {
        throw yaml::YAMLException(node.Mark(), "value cannot be < 0");
    }

    if (ms > std::numeric_limits<uint32_t>::max()) {
        throw yaml::YAMLException(node.Mark(), "value exceeds limits of uint32_t");
    }

    return static_cast<uint32_t>(ms);
}

ssp21::SessionLimits get_session_limits(const YAML::Node& node)
{
    ssp21::SessionLimits limits;

    limits.max_nonce_value = yaml::optional_integer<uint16_t>(node, "max_nonce_value", limits.max_nonce_value);
    limits.max_session_time_ms = yaml::optional_integer<uint32_t>(node, "max_session_time_ms", limits.max_session_time_ms);

    return limits;
}

ssp21::CryptoLayerConfig get_crypto_layer_config(const YAML::Node& node)
{
    ssp21::CryptoLayerConfig config;

    config.max_payload_size = yaml::optional_integer<uint16_t>(node, "max_payload_size", config.max_payload_size);

    return config;
}

ssp21::SessionConfig get_session_config(const YAML::Node& node)
{
    ssp21::SessionConfig config;

    config.ttl_pad_ms = get_optional_ms_from_duration(node, "ttl_pad", config.ttl_pad_ms);

    return config;
}

ssp21::InitiatorConfig::Params get_initiator_params(const YAML::Node& handshake, const YAML::Node& session)
{
    ssp21::InitiatorConfig::Params config;

    config.nonce_renegotiation_trigger_value = yaml::optional_integer<uint16_t>(session, "nonce_renegotiation_trigger_value", config.nonce_renegotiation_trigger_value);
    config.response_timeout = yaml::optional_duration(handshake, "response_timeout", config.response_timeout);
    config.retry_timeout = yaml::optional_duration(handshake, "retry_timeout", config.retry_timeout);
    config.session_time_renegotiation_trigger_ms = get_optional_ms_from_duration(session, "session_time_renegotiation_trigger_ms", config.session_time_renegotiation_trigger_ms);

    return config;
}

ssp21::InitiatorConfig get_initiator_config(const YAML::Node& handshake, const YAML::Node& session)
{
    // all of the configuration here is optional and uses the defaults if not present
    ssp21::InitiatorConfig config;

    config.config = get_crypto_layer_config(session);
    config.session = get_session_config(session);
    config.session_limits = get_session_limits(session);
    config.params = get_initiator_params(handshake, session);

    return config;
}

ssp21::ResponderConfig get_responder_config(const YAML::Node& session)
{
    // all of the configuration here is optional and uses the defaults if not present
    ssp21::ResponderConfig config;

    config.config = get_crypto_layer_config(session);
    config.session = get_session_config(session);

    return config;
}

ssp21::StaticKeys get_local_static_keys(const YAML::Node& node)
{
    return ssp21::StaticKeys(
        get_crypto_key<PublicKey>(yaml::require_string(node, "local_public_key_path"), ContainerEntryType::x25519_public_key),
        get_crypto_key<PrivateKey>(yaml::require_string(node, "local_private_key_path"), ContainerEntryType::x25519_private_key));
}

stack_factory_t get_initiator_shared_secret_factory(const YAML::Node& node, const ssp21::InitiatorConfig& initiator_config, const ssp21::Addresses* addresses)
{
    const auto shared_secret = get_shared_secret(node);

    if (addresses) {
        const auto addresses_copy = *addresses;
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::nonce;

            return initiator::factory::shared_secret_mode(
                addresses_copy,
                initiator_config,
                logger,
                executor,
                suite,
                shared_secret);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::nonce;

            return initiator::factory::shared_secret_mode(
                initiator_config,
                logger,
                executor,
                suite,
                shared_secret);
        };
    }
}

stack_factory_t get_initiator_qkd_factory(const YAML::Node& node, const ssp21::InitiatorConfig& initiator_config, const ssp21::Addresses* addresses)
{
    const auto key_source = QKDSourceRegistry::get_initiator_key_source(node);

    if (addresses) {
        const auto addresses_copy = *addresses;

        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::none;

            return initiator::factory::qkd_mode(
                addresses_copy,
                initiator_config,
                logger,
                executor,
                suite,
                key_source);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::none;

            return initiator::factory::qkd_mode(
                initiator_config,
                logger,
                executor,
                suite,
                key_source);
        };
    }
}

stack_factory_t get_initiator_preshared_public_key_factory(const YAML::Node& node, const ssp21::InitiatorConfig& initiator_config, const ssp21::Addresses* addresses)
{
    const auto local_keys = get_local_static_keys(node);
    const auto remote_public_key = get_crypto_key<ssp21::PublicKey>(yaml::require_string(node, "remote_public_key_path"), ContainerEntryType::x25519_public_key);

    if (addresses) {
        const auto addresses_copy = *addresses;
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::nonce;

            return initiator::factory::preshared_public_key_mode(
                addresses_copy,
                initiator_config,
                logger,
                executor,
                CryptoSuite(), // TODO: default
                local_keys,
                remote_public_key);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::nonce;

            return initiator::factory::preshared_public_key_mode(
                initiator_config,
                logger,
                executor,
                CryptoSuite(), // TODO: default
                local_keys,
                remote_public_key);
        };
    }
}

stack_factory_t get_initiator_certificate_mode_factory(const YAML::Node& node, const ssp21::InitiatorConfig& initiator_config, const ssp21::Addresses* addresses)
{
    const auto local_keys = get_local_static_keys(node);
    const auto anchor_cert_data = get_file_data(node, "authority_cert_path");
    const auto local_cert_data = get_file_data(node, "local_cert_path");

    if (addresses) {
        const auto addresses_copy = *addresses;
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::nonce;

            return initiator::factory::certificate_public_key_mode(
                addresses_copy,
                initiator_config,
                logger,
                executor,
                CryptoSuite(), // TODO: default
                local_keys,
                anchor_cert_data,
                local_cert_data);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            CryptoSuite suite;
            suite.handshake_ephemeral = HandshakeEphemeral::nonce;

            return initiator::factory::certificate_public_key_mode(
                initiator_config,
                logger,
                executor,
                CryptoSuite(), // TODO: default
                local_keys,
                anchor_cert_data,
                local_cert_data);
        };
    }
}

stack_factory_t get_initiator_factory(const YAML::Node& node, const ssp21::Addresses* addresses)
{
    const auto handshake = yaml::require(node, "handshake");
    const auto session = yaml::require(node, "session");
    const auto config = get_initiator_config(handshake, session);

    const auto mode = get_handshake_mode(handshake);

    switch (mode) {
    case (HandshakeMode::shared_secret):
        return get_initiator_shared_secret_factory(handshake, config, addresses);
    case (HandshakeMode::quantum_key_distribution):
        return get_initiator_qkd_factory(handshake, config, addresses);
    case (HandshakeMode::preshared_public_keys):
        return get_initiator_preshared_public_key_factory(handshake, config, addresses);
    case (HandshakeMode::industrial_certificates):
        return get_initiator_certificate_mode_factory(handshake, config, addresses);
    default:
        throw Exception("unsupported initiator handshake mode: ", HandshakeModeSpec::to_string(mode));
    }
}

stack_factory_t get_responder_shared_secret_factory(const YAML::Node& node, const ResponderConfig& config, const ssp21::Addresses* addresses)
{
    const auto shared_secret = get_shared_secret(node);

    if (addresses) {
        const auto addresses_copy = *addresses;
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::shared_secret_mode(
                addresses_copy,
                config,
                logger,
                executor,
                shared_secret);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::shared_secret_mode(
                config,
                logger,
                executor,
                shared_secret);
        };
    }
}

stack_factory_t get_responder_qkd_factory(const YAML::Node& node, const ResponderConfig& config, const ssp21::Addresses* addresses)
{
    const auto key_lookup = QKDSourceRegistry::get_responder_key_lookup(node);

    if (addresses) {
        const auto addresses_copy = *addresses;
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::qkd_mode(
                addresses_copy,
                config,
                logger,
                executor,
                key_lookup);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::qkd_mode(
                config,
                logger,
                executor,
                key_lookup);
        };
    }
}

stack_factory_t get_responder_preshared_public_key_factory(const YAML::Node& node, const ResponderConfig& config, const ssp21::Addresses* addresses)
{
    const auto local_keys = get_local_static_keys(node);
    const auto remote_public_key = get_crypto_key<ssp21::PublicKey>(yaml::require_string(node, "remote_public_key_path"), ContainerEntryType::x25519_public_key);

    if (addresses) {
        const auto addresses_copy = *addresses;
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::preshared_public_key_mode(
                addresses_copy,
                config,
                logger,
                executor,
                local_keys,
                remote_public_key);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::preshared_public_key_mode(
                config,
                logger,
                executor,
                local_keys,
                remote_public_key);
        };
    }
}

stack_factory_t get_responder_certificate_mode_factory(const YAML::Node& node, const ResponderConfig& config, const ssp21::Addresses* addresses)
{
    const auto local_keys = get_local_static_keys(node);
    const auto anchor_cert_data = get_file_data(node, "authority_cert_path");
    const auto local_cert_data = get_file_data(node, "local_cert_path");

    if (addresses) {
        const auto addresses_copy = *addresses;
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::certificate_public_key_mode(
                addresses_copy,
                config,
                logger,
                executor,
                local_keys,
                anchor_cert_data,
                local_cert_data);
        };
    } else {
        return [=](const log4cpp::Logger& logger, const std::shared_ptr<exe4cpp::IExecutor>& executor) {
            return responder::factory::certificate_public_key_mode(
                config,
                logger,
                executor,
                local_keys,
                anchor_cert_data,
                local_cert_data);
        };
    }
}

stack_factory_t get_responder_factory(const YAML::Node& node, const ssp21::Addresses* addresses)
{
    const auto handshake = yaml::require(node, "handshake");
    const auto config = get_responder_config(yaml::require(node, "session"));
    const auto mode = get_handshake_mode(handshake);

    switch (mode) {

    case (HandshakeMode::shared_secret):
        return get_responder_shared_secret_factory(handshake, config, addresses);
    case (HandshakeMode::quantum_key_distribution):
        return get_responder_qkd_factory(handshake, config, addresses);
    case (HandshakeMode::preshared_public_keys):
        return get_responder_preshared_public_key_factory(handshake, config, addresses);
    case (HandshakeMode::industrial_certificates):
        return get_responder_certificate_mode_factory(handshake, config, addresses);
    default:
        throw Exception("unsupported responder handshake mode: ", HandshakeModeSpec::to_string(mode));
    }
}

StackFactory get_stack_factory(const YAML::Node& node)
{
    const auto link_layer = yaml::require(node, "link_layer");
    const auto security = yaml::require(node, "security");
    const auto stack_type = get_stack_type(security);

    if (yaml::require_bool(link_layer, "enabled")) {
        const auto addresses = get_addresses(yaml::require(link_layer, "address"));
        if (stack_type == StackType::initiator) {
            return StackFactory(true, stack_type, get_initiator_factory(security, &addresses));
        } else {
            return StackFactory(true, stack_type, get_responder_factory(security, &addresses));
        }
    } else {
        if (stack_type == StackType::initiator) {
            return StackFactory(false, stack_type, get_initiator_factory(security, nullptr));
        } else {
            return StackFactory(false, stack_type, get_responder_factory(security, nullptr));
        }
    }
}

}
