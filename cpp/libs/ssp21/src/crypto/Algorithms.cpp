

#include "ssp21/util/Exception.h"
#include "crypto/Algorithms.h"

namespace ssp21
{
    HandshakeError Algorithms::Session::configure(SessionNonceMode nonce_mode, SessionMode session_mode)
    {
        switch (nonce_mode)
        {
        case(SessionNonceMode::greater_than_last_rx):
            this->verify_nonce = &NonceFunctions::verify_greater_than_last;
            break;
        case(SessionNonceMode::increment_last_rx):
            this->verify_nonce = &NonceFunctions::verify_strict_increment;
            break;
        default:
            return HandshakeError::unsupported_nonce_mode;
        }

        switch (session_mode)
        {
        case(SessionMode::hmac_sha256_16):
            this->session_mode = &SessionModes::get_hmac_sha_256_trunc16();
            break;
        default:
            return HandshakeError::unsupported_session_mode;
        }

        return HandshakeError::none;
    }

    HandshakeError Algorithms::SharedSecretHandshake::configure(
        HandshakeEphemeral handshake_ephemeral,
        HandshakeKDF handshake_kdf,
        HandshakeHash handshake_hash
    )
    {
        switch (handshake_hash)
        {
        case(HandshakeHash::sha256):
            this->hash = &Crypto::hash_sha256;
            break;
        default:
            return HandshakeError::unsupported_handshake_hash;
        }

        switch (handshake_kdf)
        {
        case(HandshakeKDF::hkdf_sha256):
            this->kdf = &Crypto::hkdf_sha256;
            break;
        default:
            return HandshakeError::unsupported_handshake_kdf;
        }

        return HandshakeError::none;
    }

    HandshakeError Algorithms::PublicKeyHandshake::configure(
        HandshakeEphemeral handshake_ephemeral,
        HandshakeKDF handshake_kdf,
        HandshakeHash handshake_hash
    )
    {
        {
            const auto err = SharedSecretHandshake::configure(handshake_ephemeral, handshake_kdf, handshake_hash);
            if (any(err)) return err;
        }

        switch (handshake_ephemeral)
        {
        case(HandshakeEphemeral::x25519):
            this->dh = &Crypto::dh_x25519;
            this->gen_keypair = &Crypto::gen_keypair_x25519;
            break;
        default:
            return HandshakeError::unsupported_handshake_ephemeral;
        }

        return HandshakeError::none;
    }

}


