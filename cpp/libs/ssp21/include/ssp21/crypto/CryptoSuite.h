
#ifndef SSP21_CRYPTOSUITE_H
#define SSP21_CRYPTOSUITE_H

#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/HandshakeEphemeral.h"
#include "ssp21/crypto/gen/HandshakeHash.h"
#include "ssp21/crypto/gen/HandshakeKDF.h"
#include "ssp21/crypto/gen/SessionMode.h"

namespace ssp21
{
    // The cyrptographic modes that the initiator will request from the responder
    struct CryptoSuite
    {
        CryptoSuite() = default;

        CryptoSuite(
            NonceMode nonce_mode,
            HandshakeEphemeral handshake_ephemeral,
            HandshakeHash handshake_hash,
            HandshakeKDF handshake_kdf,
            SessionMode session_mode
        ) :
            nonce_mode(nonce_mode),
            handshake_ephemeral(handshake_ephemeral),
            handshake_hash(handshake_hash),
            handshake_kdf(handshake_kdf),
            session_mode(session_mode)
        {}

        NonceMode nonce_mode = NonceMode::increment_last_rx;
        HandshakeEphemeral handshake_ephemeral = HandshakeEphemeral::x25519;
        HandshakeHash handshake_hash = HandshakeHash::sha256;
        HandshakeKDF handshake_kdf = HandshakeKDF::hkdf_sha256;
        SessionMode session_mode = SessionMode::hmac_sha256_16;
    };

}

#endif
