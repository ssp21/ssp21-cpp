
#include "crypto/ICertificateHandler.h"

#include "crypto/PresharedKeyCertificateHandler.h"
#include "crypto/IndustrialCertificateHandler.h"

namespace ssp21
{

    std::shared_ptr<ICertificateHandler> ICertificateHandler::preshared_key(const std::shared_ptr<const PublicKey>& remote_static_public_key)
    {
        return std::make_shared<PresharedKeyCertificateHandler>(remote_static_public_key);
    }

    std::shared_ptr<ICertificateHandler> ICertificateHandler::certificates(
        const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
        const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data
    )
    {
        return std::make_shared<IndustrialCertificateHandler>(anchor_cert_file_data, presented_chain_file_data);
    }
}


