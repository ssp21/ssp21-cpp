
#include "ssp21/crypto/ResponderHandshakes.h"

#include "ssp21/crypto/ResponderHandshake.h"

namespace ssp21
{
    std::shared_ptr<IResponderHandshake> ResponderHandshakes::public_key_mode(
        const openpal::Logger& logger,
        const StaticKeys& static_keys,
        const std::shared_ptr<ICertificateHandler>& cert_handler)
    {
        return ResponderHandshake::create_shared(logger, static_keys, cert_handler);
    }






}


