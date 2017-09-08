
#include "ssp21/stack/Factory.h"

#include "ssp21/stack/LinkCryptoStack.h"

namespace ssp21
{

    std::shared_ptr<IStack> Factory::responder(
        Addresses addresses,
        const ResponderConfig& config,
        openpal::Logger logger,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& certificate_handler)
    {
        return std::make_shared<ResponderStack>(addresses, config, logger, executor, static_keys, certificate_handler);
    }

    std::shared_ptr<IStack> Factory::initiator(
        Addresses addresses,
        const InitiatorConfig& config,
		const CryptoSuite& crypto_suite,
        openpal::Logger logger,
        const std::shared_ptr<openpal::IExecutor>& executor,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& certificate_handler)
    {
        return std::make_shared<InitiatorStack>(addresses, config, crypto_suite, logger, executor, static_keys, certificate_handler);
    }

}

