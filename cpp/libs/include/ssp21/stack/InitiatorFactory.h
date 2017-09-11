
#ifndef SSP21_INITIATORFACTORY_H
#define SSP21_INITIATORFACTORY_H

#include "ssp21/stack/IStack.h"

#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "ssp21/crypto/InitiatorHandshake.h"

#include "ssp21/link/Addresses.h"

#include "openpal/logging/Logger.h"
#include "openpal/executor/IExecutor.h"

#include <memory>

namespace ssp21
{
    class InitiatorFactory final
    {
    public:

        static std::shared_ptr<IStack> preshared_public_key_mode(
            const Addresses& addresses,
            const InitiatorConfig& config,
            const openpal::Logger& logger,
            const std::shared_ptr<openpal::IExecutor>& executor,
			const CryptoSuite& crypto_suite,
			const StaticKeys& local_keys,
			const std::shared_ptr<const PublicKey>& remote_public_key
        );

		static std::shared_ptr<IStack> certificate_public_key_mode(
			const Addresses& addresses,
			const InitiatorConfig& config,
			const openpal::Logger& logger,
			const std::shared_ptr<openpal::IExecutor>& executor,
			const CryptoSuite& crypto_suite,
			const StaticKeys& local_keys,
			const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
			const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data			
		);
    };
}

#endif
