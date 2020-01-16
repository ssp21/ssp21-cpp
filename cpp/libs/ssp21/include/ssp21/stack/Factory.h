#ifndef SSP21_FACTORY_H
#define SSP21_FACTORY_H

/** @file
 * @brief Factory methods to create a protocol stack.
 * 
 * To create an initiator, use one of the following functions:
 * - @ref ssp21::initiator::factory::shared_secret_mode()
 * - @ref ssp21::initiator::factory::qkd_mode()
 * - @ref ssp21::initiator::factory::preshared_public_key_mode()
 * - @ref ssp21::initiator::factory::certificate_public_key_mode()
 * 
 * To create a responder, use one of the following functions:
 * - @ref ssp21::responder::factory::shared_secret_mode()
 * - @ref ssp21::responder::factory::preshared_public_key_mode()
 * - @ref ssp21::responder::factory::qkd_mode()
 * - @ref ssp21::responder::factory::certificate_public_key_mode()
 */

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "ssp21/crypto/IKeyLookup.h"
#include "ssp21/crypto/IKeySource.h"
#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/link/Addresses.h"
#include "ssp21/stack/IStack.h"
#include "ssp21/util/SecureDynamicBuffer.h"

#include "exe4cpp/IExecutor.h"
#include "log4cpp/Logger.h"

#include <memory>

namespace ssp21 {
/**
     * @brief Responder namespace.
     */
namespace responder {
    /**
         * @brief Responder factory functions.
         */
    namespace factory {
        /**
             * @brief Create a shared secret responder stack.
             * @param addresses Link-layer addresses used
             * @param config    Responder configuration
             * @param logger    Logger used by the stack
             * @param executor  Executor used by the stack
             * @param key       Shared secret
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> shared_secret_mode(
            const Addresses& addresses,
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<const SymmetricKey>& key);

        /**
             * @brief Create a shared secret responder stack without the link-layer.
             * @param config    Responder configuration
             * @param logger    Logger used by the stack
             * @param executor  Executor used by the stack
             * @param key       Shared secret
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> shared_secret_mode(
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<const SymmetricKey>& key);

        /**
             * @brief Create a quantum key destribution (QKD) responder stack.
             * @param addresses  Link-layer addresses used
             * @param config     Responder configuration
             * @param logger     Logger used by the stack
             * @param executor   Executor used by the stack
             * @param key_lookup Key lookup interface
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> qkd_mode(
            const Addresses& addresses,
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<IKeyLookup>& key_lookup);

        /**
             * @brief Create a quantum key destribution (QKD) responder stack without the link-layer.
             * @param config     Responder configuration
             * @param logger     Logger used by the stack
             * @param executor   Executor used by the stack
             * @param key_lookup Key lookup interface
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> qkd_mode(
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const std::shared_ptr<IKeyLookup>& key_lookup);

        /**
             * @brief Create a preshared public key responder stack.
             * @param addresses         Link-layer addresses used
             * @param config            Responder configuration
             * @param logger            Logger used by the stack
             * @param executor          Executor used by the stack
             * @param local_keys        Local key pair
             * @param remote_public_key Remote preshared public key
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> preshared_public_key_mode(
            const Addresses& addresses,
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const StaticKeys& local_keys,
            const std::shared_ptr<const PublicKey>& remote_public_key);

        /**
             * @brief Create a preshared public key responder stack without the link-layer
             * @param config            Responder configuration
             * @param logger            Logger used by the stack
             * @param executor          Executor used by the stack
             * @param local_keys        Local key pair
             * @param remote_public_key Remote preshared public key
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> preshared_public_key_mode(
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const StaticKeys& local_keys,
            const std::shared_ptr<const PublicKey>& remote_public_key);

        /**
             * @brief Create a certificate-based responder stack.
             * @param addresses                 Link-layer addresses used
             * @param config                    Responder configuration
             * @param logger                    Logger used by the stack
             * @param executor                  Executor used by the stack
             * @param local_keys                Local key pair
             * @param anchor_cert_file_data     Root certificate file
             * @param presented_chain_file_data Certificate chain to present to the remote
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> certificate_public_key_mode(
            const Addresses& addresses,
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const StaticKeys& local_keys,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data);

        /**
             * @brief Create a certificate-based responder stack without the link-layer
             * @param config                    Responder configuration
             * @param logger                    Logger used by the stack
             * @param executor                  Executor used by the stack
             * @param local_keys                Local key pair
             * @param anchor_cert_file_data     Root certificate file
             * @param presented_chain_file_data Certificate chain to present to the remote
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             */
        std::shared_ptr<IStack> certificate_public_key_mode(
            const ResponderConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const StaticKeys& local_keys,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data);

    }

}

/**
     * @brief Initiator namespace.
     */
namespace initiator {
    /**
         * @brief Initiator factory functions.
         */
    namespace factory {
        /**
             * @brief Create a shared secret initiator stack.
             * @param addresses    Link-layer addresses used
             * @param config       Responder configuration
             * @param logger       Logger used by the stack
             * @param executor     Executor used by the stack
             * @param crypto_suite Cryptographic modes that will be requested
             * @param key          Shared secret
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::nonce.
             */
        std::shared_ptr<IStack> shared_secret_mode(
            const Addresses& addresses,
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const std::shared_ptr<const SymmetricKey>& key);

        /**
             * @brief Create a shared secret initiator stack without the link-layer.
             * @param config       Responder configuration
             * @param logger       Logger used by the stack
             * @param executor     Executor used by the stack
             * @param crypto_suite Cryptographic modes that will be requested
             * @param key          Shared secret
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::nonce.
             */
        std::shared_ptr<IStack> shared_secret_mode(
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const std::shared_ptr<const SymmetricKey>& key);

        /**
             * @brief Create a quantum key distribution (QKD) initiator stack.
             * @param addresses    Link-layer addresses used
             * @param config       Responder configuration
             * @param logger       Logger used by the stack
             * @param executor     Executor used by the stack
             * @param crypto_suite Cryptographic modes that will be requested
             * @param key_source   Key source interface
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::none.
             */
        std::shared_ptr<IStack> qkd_mode(
            const Addresses& addresses,
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const std::shared_ptr<IKeySource>& key_source);

        /**
             * @brief Create a quantum key distribution (QKD) initiator stack without the link-layer.
             * @param config       Responder configuration
             * @param logger       Logger used by the stack
             * @param executor     Executor used by the stack
             * @param crypto_suite Cryptographic modes that will be requested
             * @param key_source   Key source interface
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::none.
             */
        std::shared_ptr<IStack> qkd_mode(
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const std::shared_ptr<IKeySource>& key_source);

        /**
             * @brief Create a preshared public key initiator stack.
             * @param addresses         Link-layer addresses used
             * @param config            Responder configuration
             * @param logger            Logger used by the stack
             * @param executor          Executor used by the stack
             * @param crypto_suite      Cryptographic modes that will be requested
             * @param local_keys        Local key pair
             * @param remote_public_key Remote public key
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::x25519.
             */
        std::shared_ptr<IStack> preshared_public_key_mode(
            const Addresses& addresses,
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const StaticKeys& local_keys,
            const std::shared_ptr<const PublicKey>& remote_public_key);

        /**
             * @brief Create a preshared public key initiator stack without the link-layer.
             * @param config            Responder configuration
             * @param logger            Logger used by the stack
             * @param executor          Executor used by the stack
             * @param crypto_suite      Cryptographic modes that will be requested
             * @param local_keys        Local key pair
             * @param remote_public_key Remote public key
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::x25519.
             */
        std::shared_ptr<IStack> preshared_public_key_mode(
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const StaticKeys& local_keys,
            const std::shared_ptr<const PublicKey>& remote_public_key);

        /**
             * @brief Create a certificate-based initiator stack.
             * @param addresses                 Link-layer addresses used
             * @param config                    Responder configuration
             * @param logger                    Logger used by the stack
             * @param executor                  Executor used by the stack
             * @param crypto_suite              Cryptographic modes that will be requested
             * @param local_keys                Local key pair
             * @param anchor_cert_file_data     Root certificate file
             * @param presented_chain_file_data Certificate chain to present to the remote
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::x25519.
             */
        std::shared_ptr<IStack> certificate_public_key_mode(
            const Addresses& addresses,
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const StaticKeys& local_keys,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data);

        /**
             * @brief Create a certificate-based initiator stack without the link-layer.
             * @param config                    Responder configuration
             * @param logger                    Logger used by the stack
             * @param executor                  Executor used by the stack
             * @param crypto_suite              Cryptographic modes that will be requested
             * @param local_keys                Local key pair
             * @param anchor_cert_file_data     Root certificate file
             * @param presented_chain_file_data Certificate chain to present to the remote
             * @return Stack to which an @ref IUpperLayer and an @ref ILowerLayer must be bind
             * 
             * @note The only valid @ref HandshakeEphemeral for this mode is @ref HandshakeEphemeral::x25519.
             */
        std::shared_ptr<IStack> certificate_public_key_mode(
            const InitiatorConfig& config,
            const log4cpp::Logger& logger,
            const std::shared_ptr<exe4cpp::IExecutor>& executor,
            const CryptoSuite& crypto_suite,
            const StaticKeys& local_keys,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
            const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data);

    }
}
}

#endif
