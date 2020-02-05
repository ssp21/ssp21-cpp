
#ifndef SSP21_CRYPTOSUITE_H
#define SSP21_CRYPTOSUITE_H

#include "ssp21/crypto/gen/HandshakeEphemeral.h"
#include "ssp21/crypto/gen/HandshakeHash.h"
#include "ssp21/crypto/gen/HandshakeKDF.h"
#include "ssp21/crypto/gen/SessionCryptoMode.h"
#include "ssp21/crypto/gen/SessionNonceMode.h"

namespace ssp21 {

// The cyrptographic primitives that the initiator always requests
// the responder to use. Mode-specific primitives may also be used
// but are specified in the opaque 'mode_data' fields
struct CryptoSuite {
    CryptoSuite() = default;

    CryptoSuite(
        HandshakeHash handshake_hash,
        HandshakeKDF handshake_kdf,
        SessionNonceMode session_nonce_mode,
        SessionCryptoMode session_crypto_mode)
        : handshake_hash(handshake_hash)
        , handshake_kdf(handshake_kdf)
        , session_nonce_mode(session_nonce_mode)
        , session_crypto_mode(session_crypto_mode)
    {
    }

    HandshakeHash handshake_hash = HandshakeHash::sha256;
    HandshakeKDF handshake_kdf = HandshakeKDF::hkdf_sha256;
    SessionNonceMode session_nonce_mode = SessionNonceMode::strict_increment;
    SessionCryptoMode session_crypto_mode = SessionCryptoMode::hmac_sha256_16;
};

struct DHCryptoSuite {
    DHCryptoSuite() = default;

    DHCryptoSuite(CryptoSuite base, HandshakeEphemeral handshake_ephemeral)
        : base(base)
        , handshake_ephemeral(handshake_ephemeral)
    {
    }

    CryptoSuite base;
    HandshakeEphemeral handshake_ephemeral = HandshakeEphemeral::x25519;
};

}

#endif
