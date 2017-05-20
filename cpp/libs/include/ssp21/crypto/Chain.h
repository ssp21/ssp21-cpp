
#ifndef SSP21_CHAIN_H
#define SSP21_CHAIN_H

#include "ssp21/crypto/gen/CertificateBody.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/CryptoTypedefs.h"

#include <openpal/util/Uncopyable.h>

namespace ssp21
{
    /**
    * Operations for verifying certificate chains
    */
    class Chain final : openpal::StaticOnly
    {

    public:

        /**
        *
        * Using the previously verified parent certifivate, verify the next untrusted child certificate
        *
        * @param parent trusted parent certificate body
        * @param child certificate to be verified
        * @param child_body if verification is successful, the fully parsed child certificate body is output in this parameter
        * @return An error condition. HandshakeError::none, if the verification was successful
        *
        */
        static HandshakeError verify_pair(const CertificateBody& parent, const CertificateEnvelope& child, CertificateBody& child_body);

    private:

        struct DSAInfo
        {
            verify_dsa_t verify;
            uint8_t signature_length;
        };

        static DSAInfo try_get_dsa_info(PublicKeyType type);
    };
}

#endif
