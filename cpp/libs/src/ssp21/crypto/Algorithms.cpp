

#include "ssp21/crypto/Algorithms.h"

namespace ssp21
{
    HandshakeError Algorithms::configure(const CryptoSpec& spec)
    {
        Algorithms algorithms;

        switch (spec.dh_mode)
        {
        case(DHMode::x25519):
            algorithms.handshake.dh = &Crypto::dh_x25519;
            algorithms.handshake.gen_keypair = &Crypto::gen_keypair_x25519;
            break;
        default:
            return HandshakeError::unsupported_dh_mode;
        }

        switch (spec.handshake_hash)
        {
        case(HandshakeHash::sha256):
            algorithms.handshake.hash = &Crypto::hash_sha256;
            break;
        default:
            return HandshakeError::unsupported_handshake_hash;
        }

        switch (spec.handshake_kdf)
        {
        case(HandshakeKDF::hkdf_sha256):
            algorithms.handshake.kdf = &Crypto::hkdf_sha256;
            break;
        default:
            return HandshakeError::unsupported_handshake_kdf;
        }       

        switch (spec.nonce_mode)
        {
        case(NonceMode::greater_than_last_rx):
            algorithms.session.verify_nonce = &NonceFunctions::verify_greater_than_last;
            break;
        case(NonceMode::increment_last_rx):
            algorithms.session.verify_nonce = &NonceFunctions::verify_strict_increment;
            break;
        default:
            return HandshakeError::unsupported_nonce_mode;
        }

        switch (spec.session_mode)
        {
        case(SessionMode::hmac_sha256_16):
            algorithms.session.mode = &SessionModes::get_hmac_sha_256_trunc16();
            break;
        default:
            return HandshakeError::unsupported_session_mode;
        }

        (*this) = algorithms;

        return HandshakeError::none;
    }

}


