
#ifndef SSP21_ALGORITHMS_H
#define SSP21_ALGORITHMS_H

#include "CryptoTypedefs.h"

#include "ssp21/crypto/NonceFunctions.h"
#include "ssp21/crypto/SessionModes.h"
#include "ssp21/crypto/HandshakeAuthentication.h"

#include "ssp21/crypto/gen/DHMode.h"
#include "ssp21/crypto/gen/HashMode.h"
#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/SessionMode.h"

#include "ssp21/crypto/Crypto.h"

namespace ssp21
{
    /**
    * Represents a complete set of algorithms for the handshake and the session
    */
    struct Algorithms
    {

    public:

        struct Config
        {
            Config(
                DHMode dh_mode,
                HashMode hash_mode,
                NonceMode nonce_mode,
                SessionMode session_mode) :
                dh_mode(dh_mode),
                hash_mode(hash_mode),
                nonce_mode(nonce_mode),
                session_mode(session_mode)
            {}

            DHMode dh_mode;
            HashMode hash_mode;
            NonceMode nonce_mode;
            SessionMode session_mode;
        };

        struct Session
        {
            Session() = default;

            verify_nonce_func_t verify_nonce = NonceFunctions::default_verify();
			ISessionMode* mode = &SessionModes::default_mode();
        };

        struct Handshake
        {
            Handshake() = default;

            dh_func_t dh = &Crypto::dh_x25519;
            hkdf_func_t hkdf = &Crypto::hkdf_sha256;
            hash_func_t hash = &Crypto::hash_sha256;
            mac_func_t session_auth_mac = &Crypto::hmac_sha256;
            gen_keypair_func_t gen_keypair = &Crypto::gen_keypair_x25519;
            auth_handshake_t auth_handshake = HandshakeAuthentication::default_auth_handshake();
            calc_handshake_mac_t calc_handshake_mac = HandshakeAuthentication::default_calc_handshake_mac();
        };

        // default constructor initializes with default algorithms
        Algorithms() = default;

        HandshakeError configure(const Config& config);

        // handshake algorithms
        Handshake handshake;

        // session algorithms
        Session session;
    };


}

#endif
