
#ifndef SSP21_ALGORITHMS_H
#define SSP21_ALGORITHMS_H

#include "ssp21/crypto/CryptoTypedefs.h"
#include "ssp21/crypto/NonceFunctions.h"
#include "ssp21/crypto/SessionModes.h"
#include "ssp21/crypto/CryptoSuite.h"

#include "ssp21/crypto/gen/CryptoSpec.h"
#include "ssp21/crypto/gen/HandshakeError.h"

#include "ssp21/crypto/Crypto.h"


namespace ssp21
{
    /**
    * Represents a complete set of algorithms for the handshake and the session
    */
    struct Algorithms
    {

    public:

        struct Session
        {
            Session() = default;

            HandshakeError configure(
                NonceMode nonce_mode,
                SessionMode session_mode
            );

            verify_nonce_func_t verify_nonce = NonceFunctions::default_verify();
            ISessionMode* session_mode = &SessionModes::default_mode();
        };

        struct Handshake
        {
            Handshake() = default;

            HandshakeError configure(
                DHMode dh_mode,
                HandshakeKDF handshake_kdf,
                HandshakeHash handshake_hash
            );

            dh_func_t dh = &Crypto::dh_x25519;
            kdf_func_t kdf = &Crypto::hkdf_sha256;
            hash_func_t hash = &Crypto::hash_sha256;
            gen_keypair_func_t gen_keypair = &Crypto::gen_keypair_x25519;
        };

        // default constructor initializes with default algorithms
        Algorithms() = default;

        // construct the algorithms
        Algorithms(const CryptoSuite& suite);

        // configure the algorithms from a received CryptoSpec
        HandshakeError configure(const CryptoSpec& spec);

        // handshake algorithms
        Handshake handshake;

        // session algorithms
        Session session;
    };


}

#endif
