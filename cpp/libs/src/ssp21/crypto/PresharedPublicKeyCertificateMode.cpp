

#include "ssp21/crypto/PresharedPublicKeyCertificateMode.h"

namespace ssp21
{

    PresharedPublicKeyCertificateMode::PresharedPublicKeyCertificateMode(
        const std::shared_ptr<const PublicKey>& remote_static_public_key
    ) :
        remote_static_public_key(remote_static_public_key)
    {

    }

    HandshakeError PresharedPublicKeyCertificateMode::validate(const ssp21::seq32_t& certificates, seq32_t& public_key_output)
    {
        if (certificates.is_not_empty()) return HandshakeError::bad_certificate_format;

        public_key_output = this->remote_static_public_key->as_seq();

        return HandshakeError::none;
    }

}


