
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
