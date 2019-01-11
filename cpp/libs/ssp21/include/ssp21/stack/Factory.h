
#ifndef SSP21_FACTORY_H
#define SSP21_FACTORY_H

#include "ssp21/stack/IStack.h"

#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "ssp21/crypto/IKeySource.h"
#include "ssp21/crypto/IKeyLookup.h"
#include "ssp21/util/SecureDynamicBuffer.h"

#include "ssp21/link/Addresses.h"

#include "log4cpp/Logger.h"
#include "exe4cpp/IExecutor.h"

#include <memory>

namespace ssp21
{
    namespace responder
    {

        namespace factory
        {
            std::shared_ptr<IStack> shared_secret_mode(
                const Addresses& addresses,
                const ResponderConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const std::shared_ptr<const SymmetricKey>& key
            );

            std::shared_ptr<IStack> qkd_mode(
                const Addresses& addresses,
                const ResponderConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const std::shared_ptr<IKeyLookup>& key_lookup
            );

            std::shared_ptr<IStack> preshared_public_key_mode(
                const Addresses& addresses,
                const ResponderConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const StaticKeys& local_keys,
                const std::shared_ptr<const PublicKey>& remote_public_key
            );

            std::shared_ptr<IStack> certificate_public_key_mode(
                const Addresses& addresses,
                const ResponderConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const StaticKeys& local_keys,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
            );

        }

    }

    namespace initiator
    {
        namespace factory
        {
            std::shared_ptr<IStack> shared_secert_mode(
                const Addresses& addresses,
                const InitiatorConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const CryptoSuite& crypto_suite,
                const std::shared_ptr<const SymmetricKey>& key
            );

            std::shared_ptr<IStack> qkd_mode(
                const Addresses& addresses,
                const InitiatorConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const CryptoSuite& crypto_suite,
                const std::shared_ptr<IKeySource>& key_source
            );

            std::shared_ptr<IStack> preshared_public_key_mode(
                const Addresses& addresses,
                const InitiatorConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const CryptoSuite& crypto_suite,
                const StaticKeys& local_keys,
                const std::shared_ptr<const PublicKey>& remote_public_key
            );

            std::shared_ptr<IStack> certificate_public_key_mode(
                const Addresses& addresses,
                const InitiatorConfig& config,
                const log4cpp::Logger& logger,
                const std::shared_ptr<exe4cpp::IExecutor>& executor,
                const CryptoSuite& crypto_suite,
                const StaticKeys& local_keys,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
                const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
            );

        }
    }
}

#endif
