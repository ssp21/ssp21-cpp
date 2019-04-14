
#ifndef SSP21_ALGORITHMS_H
#define SSP21_ALGORITHMS_H

#include "ssp21/crypto/CryptoTypedefs.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "crypto/NonceFunctions.h"
#include "crypto/SessionModes.h"

#include "crypto/gen/CryptoSpec.h"
#include "ssp21/crypto/gen/HandshakeError.h"

#include "ssp21/crypto/Crypto.h"
#include "ser4cpp/util/Uncopyable.h"


namespace ssp21
{
    /**
    * Represents a complete set of algorithms for the handshake and the session
    */
    struct Algorithms : private ser4cpp::StaticOnly
    {

        struct Session
        {
            Session() = default;

            HandshakeError configure(
                SessionNonceMode nonce_mode,
				SessionCryptoMode session_mode
            );

            verify_nonce_func_t verify_nonce = NonceFunctions::default_verify();
            ISessionMode* session_mode = &SessionModes::default_mode();
        };

        struct SharedSecretHandshake
        {
            SharedSecretHandshake() = default;

            HandshakeError configure(
                HandshakeEphemeral handshake_ephemeral,
                HandshakeKDF handshake_kdf,
                HandshakeHash handshake_hash
            );

            kdf_func_t kdf = &Crypto::hkdf_sha256;
            hash_func_t hash = &Crypto::hash_sha256;
        };

        struct PublicKeyHandshake : public SharedSecretHandshake
        {
            PublicKeyHandshake() = default;

            HandshakeError configure(
                HandshakeEphemeral handshake_ephemeral,
                HandshakeKDF handshake_kdf,
                HandshakeHash handshake_hash
            );

            dh_func_t dh = &Crypto::dh_x25519;
            gen_keypair_func_t gen_keypair = &Crypto::gen_keypair_x25519;
        };

    };


}

#endif
