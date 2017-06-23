
#ifndef SSP21_FACTORY_H
#define SSP21_FACTORY_H

#include "ssp21/stack/IStack.h"

#include "ssp21/crypto/CryptoLayerConfig.h"
#include "ssp21/link/Addresses.h"
#include "ssp21/crypto/LocalKeys.h"
#include "ssp21/crypto/ICertificateMode.h"

#include "openpal/logging/Logger.h"
#include "openpal/executor/IExecutor.h"

#include <memory>

namespace ssp21
{
    class Factory final : public IStack
    {
    public:

        static std::shared_ptr<IStack> responder(
            Addresses addresses,
            const ResponderConfig& config,
            openpal::Logger logger,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const LocalKeys& keys,
            const std::shared_ptr<ICertificateMode>& certificate_mode);

        static std::shared_ptr<IStack> initiator(
            Addresses addresses,
            const InitiatorConfig& config,
            openpal::Logger logger,
            const std::shared_ptr<openpal::IExecutor>& executor,
            const LocalKeys& keys,
            const std::shared_ptr<ICertificateMode>& certificate_mode);

    };
}

#endif
