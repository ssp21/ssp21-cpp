
#include "ssp21/crypto/ICertificateHandler.h"

#include "ssp21/crypto/PresharedKeyCertificateHandler.h"

namespace ssp21
{

    std::shared_ptr<ICertificateHandler> ICertificateHandler::preshared_key(const std::shared_ptr<const PublicKey>& remote_static_public_key)
    {
        return std::make_shared<PresharedKeyCertificateHandler>(remote_static_public_key);
    }

}


