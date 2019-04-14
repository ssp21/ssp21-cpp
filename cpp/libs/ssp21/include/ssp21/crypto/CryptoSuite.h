
#ifndef SSP21_CRYPTOSUITE_H
#define SSP21_CRYPTOSUITE_H

#include "ssp21/crypto/gen/HandshakeEphemeral.h"
#include "ssp21/crypto/gen/HandshakeHash.h"
#include "ssp21/crypto/gen/HandshakeKDF.h"
#include "ssp21/crypto/gen/SessionNonceMode.h"
#include "ssp21/crypto/gen/SessionMode.h"

namespace ssp21
{
    // The cyrptographic modes that the initiator will request from the responder
    struct CryptoSuite
    {
        CryptoSuite() = default;

        CryptoSuite(            
            HandshakeEphemeral handshake_ephemeral,
            HandshakeHash handshake_hash,
            HandshakeKDF handshake_kdf,
			SessionNonceMode nonce_mode,
            SessionMode session_mode
        ) :
            nonce_mode(nonce_mode),
            handshake_ephemeral(handshake_ephemeral),
            handshake_hash(handshake_hash),
            handshake_kdf(handshake_kdf),
            session_mode(session_mode)
        {}
        
        HandshakeEphemeral handshake_ephemeral = HandshakeEphemeral::x25519;
        HandshakeHash handshake_hash = HandshakeHash::sha256;
        HandshakeKDF handshake_kdf = HandshakeKDF::hkdf_sha256;
		SessionNonceMode nonce_mode = SessionNonceMode::increment_last_rx;
        SessionMode session_mode = SessionMode::hmac_sha256_16;
    };

}

#endif
