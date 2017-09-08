
#ifndef SSP21_CRYPTOSUITE_H
#define SSP21_CRYPTOSUITE_H

#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/DHMode.h"
#include "ssp21/crypto/gen/HandshakeHash.h"
#include "ssp21/crypto/gen/HandshakeKDF.h"
#include "ssp21/crypto/gen/SessionMode.h"

namespace ssp21
{
    // The cyrptographic modes that the initiator will request from the responder
    struct CryptoSuite
    {
        NonceMode nonce_mode = NonceMode::increment_last_rx;
        DHMode dh_mode = DHMode::x25519;
        HandshakeHash handshake_hash = HandshakeHash::sha256;
        HandshakeKDF handshake_kdf = HandshakeKDF::hkdf_sha256;
        SessionMode session_mode = SessionMode::hmac_sha256_16;
    };

}

#endif
