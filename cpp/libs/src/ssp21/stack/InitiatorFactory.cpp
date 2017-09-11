
#include "ssp21/stack/InitiatorFactory.h"

#include "ssp21/stack/LinkCryptoStack.h"
#include "ssp21/crypto/InitiatorHandshakes.h"
#include "ssp21/crypto/ICertificateHandler.h"

namespace ssp21
{   
    std::shared_ptr<IStack> InitiatorFactory::preshared_public_key_mode(
        const Addresses& addresses,
        const InitiatorConfig& config,
        const openpal::Logger& logger,
        const std::shared_ptr<openpal::IExecutor>& executor,
		const CryptoSuite& crypto_suite,
		const StaticKeys& local_keys,
		const std::shared_ptr<const PublicKey>& remote_public_key)
    {
        return std::make_shared<InitiatorStack>(
                   addresses,
                   config,
                   logger,
                   executor,
				   InitiatorHandshakes::public_key_mode(
					   logger,
					   local_keys,
					   crypto_suite,
					   ICertificateHandler::preshared_key(remote_public_key)
				   )
		);
    }

	std::shared_ptr<IStack> InitiatorFactory::certificate_mode(
		const Addresses& addresses,
		const InitiatorConfig& config,
		const openpal::Logger& logger,
		const std::shared_ptr<openpal::IExecutor>& executor,
		const CryptoSuite& crypto_suite,
		const StaticKeys& local_keys,
		const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
		const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
	)
	{
		return std::make_shared<InitiatorStack>(
			addresses,
			config,
			logger,
			executor,
			InitiatorHandshakes::public_key_mode(
				logger,
				local_keys,
				crypto_suite,
				ICertificateHandler::certificates(anchor_cert_file_data, presented_chain_file_data)
			)
		);	
	}
}

