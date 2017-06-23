
#include "ssp21/crypto/ICertificateMode.h"

#include "ssp21/crypto/PresharedPublicKeyCertificateMode.h"

namespace ssp21
{

    std::shared_ptr<ICertificateMode> ICertificateMode::preshared_key(const std::shared_ptr<const PublicKey>& remote_static_public_key)
    {
        return std::make_shared<PresharedPublicKeyCertificateMode>(remote_static_public_key);
    }

}


