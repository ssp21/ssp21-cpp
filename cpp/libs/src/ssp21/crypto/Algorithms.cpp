

#include "ssp21/crypto/Algorithms.h"

#include "ssp21/crypto/Crypto.h"

namespace ssp21
{

    Algorithms::Algorithms() :
        dh(&Crypto::dh_x25519),
        hkdf(&Crypto::hkdf_sha256),
        hash(&Crypto::hash_sha256),
        gen_keypair(&Crypto::gen_keypair_x25519),
        verify_nonce(NonceFunctions::default_verify()),
        session_read(SessionModes::default_session_read())
    {}

    HandshakeError Algorithms::configure(const Config& config)
    {
        Algorithms algorithms;

        switch (config.dh_mode)
        {
        case(DHMode::x25519):
            algorithms.dh = &Crypto::dh_x25519;
            algorithms.gen_keypair = &Crypto::gen_keypair_x25519;
            break;
        default:
            return HandshakeError::unsupported_dh_mode;
        }

        switch (config.hash_mode)
        {
        case(HashMode::sha256):
            algorithms.hash = &Crypto::hash_sha256;
            algorithms.hkdf = &Crypto::hkdf_sha256;
            break;
        default:
            return HandshakeError::unsupported_hash_mode;
        }

        switch (config.nonce_mode)
        {
        case(NonceMode::greater_than_last_rx):
            algorithms.verify_nonce = &NonceFunctions::verify_greater_than_last;
            break;
        case(NonceMode::increment_last_rx):
            algorithms.verify_nonce = &NonceFunctions::verify_strict_increment;
            break;
        default:
            return HandshakeError::unsupported_nonce_mode;
        }

        switch (config.session_mode)
        {
        case(SessionMode::hmac_sha256_16):
            algorithms.session_read = &SessionModes::verify_hmac_sha256_trunc16;
            break;
        default:
            return HandshakeError::unsupported_session_mode;
        }

        (*this) = algorithms;

        return HandshakeError::none;
    }

}


