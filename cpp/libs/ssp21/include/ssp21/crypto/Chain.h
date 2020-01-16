
#ifndef SSP21_CHAIN_H
#define SSP21_CHAIN_H

#include "ssp21/crypto/CryptoTypedefs.h"
#include "ssp21/crypto/gen/CertificateBody.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/gen/HandshakeError.h"

#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {
/**
    * Operations for verifying certificate chains
    */
class Chain final : ser4cpp::StaticOnly {

public:
    /**
        * Verify a certificate chain
        *
        * @param anchor certificate to verify against
        * @param certificates Chain of unparsed certificate envelopes
        * @param result verified terminal certificate if return value is HandshakeError::none
        * @return Verification error or HandshakeError::none for success
        *
        */
    static HandshakeError verify(const CertificateBody& anchor, const ICollection<CertificateEnvelope>& certificates, CertificateBody& result);

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
    struct DSAInfo {
        verify_dsa_t verify;
        uint8_t signature_length;
    };

    static DSAInfo try_get_dsa_info(PublicKeyType type);

    static bool is_dh_key(PublicKeyType type);
};
}

#endif
