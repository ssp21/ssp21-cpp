
#ifndef SSP21_ALGORITHMS_H
#define SSP21_ALGORITHMS_H

#include "crypto/NonceFunctions.h"
#include "crypto/SessionModes.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "ssp21/crypto/CryptoTypedefs.h"

#include "crypto/gen/CryptoSpec.h"
#include "ssp21/crypto/gen/HandshakeEphemeral.h"
#include "ssp21/crypto/gen/HandshakeError.h"

#include "ser4cpp/util/Uncopyable.h"
#include "ssp21/crypto/Crypto.h"

namespace ssp21 {
/**
* Various groupings of algorithms and the functions to configure them
*/
struct Algorithms : private ser4cpp::StaticOnly {

    struct Session {
        Session() = default;

        //static Session get_or_throw(SessionNonceMode nonce_mode, SessionCryptoMode session_mode);

        HandshakeError configure(
            SessionNonceMode nonce_mode,
            SessionCryptoMode session_mode);

        verify_nonce_func_t verify_nonce = NonceFunctions::default_verify();
        SessionMode session_mode = SessionModes::default_mode();
    };

    struct Handshake {
        Handshake() = default;

        //static Handshake get_or_throw(HandshakeKDF handshake_kdf, HandshakeHash handshake_hash);

        HandshakeError configure(
            HandshakeKDF handshake_kdf,
            HandshakeHash handshake_hash);

        kdf_func_t kdf = &Crypto::hkdf_sha256;
        hash_func_t hash = &Crypto::hash_sha256;
    };

    // session + handshake algorithms common to every mode
    struct Common {
        Common() = default;

        static Common get_or_throw(const CryptoSuite& suite);

        HandshakeError configure(const CryptoSpec& spec);

        HandshakeError configure(HandshakeHash handshake_hash,
                                 HandshakeKDF handshake_kdf,
                                 SessionNonceMode session_nonce_mode,
                                 SessionCryptoMode session_crypto_mode);

        Session session;
        Handshake handshake;
    };

    struct DH {
        DH() = default;

        static DH get_or_throw(HandshakeEphemeral type);

        HandshakeError configure(HandshakeEphemeral type);

        dh_func_t dh = &Crypto::dh_x25519;
        gen_keypair_func_t gen_key_pair = &Crypto::gen_keypair_x25519;
    };
};

}

#endif
