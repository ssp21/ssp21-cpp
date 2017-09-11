
#include "ssp21/stack/ResponderFactory.h"

#include "ssp21/stack/LinkCryptoStack.h"

#include "ssp21/crypto/ResponderHandshakes.h"

namespace ssp21
{
	std::shared_ptr<IStack> ResponderFactory::preshared_public_key_mode(
		const Addresses& addresses,
		const ResponderConfig& config,
		const openpal::Logger& logger,
		const std::shared_ptr<openpal::IExecutor>& executor,
		const StaticKeys& local_keys,
		const std::shared_ptr<const PublicKey>& remote_public_key
	)
	{
		return std::make_shared<ResponderStack>(
			addresses,
			config,
			logger,
			executor,
			ResponderHandshakes::public_key_mode(
				logger,
				local_keys,
				ICertificateHandler::preshared_key(remote_public_key)
			)
		);
	}

	std::shared_ptr<IStack> ResponderFactory::certificate_public_key_mode(
		const Addresses& addresses,
		const ResponderConfig& config,
		const openpal::Logger& logger,
		const std::shared_ptr<openpal::IExecutor>& executor,
		const StaticKeys& local_keys,
		const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
		const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
	)
	{
		return std::make_shared<ResponderStack>(
			addresses,
			config,
			logger,
			executor,
			ResponderHandshakes::public_key_mode(
				logger,
				local_keys,
				ICertificateHandler::certificates(
					anchor_cert_file_data,
					presented_chain_file_data
				)
			)
		);
	}

}

