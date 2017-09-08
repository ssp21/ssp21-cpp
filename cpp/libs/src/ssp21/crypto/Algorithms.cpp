

#include "ssp21/crypto/Algorithms.h"
#include "ssp21/util/Exception.h"

namespace ssp21
{
    Algorithms::Algorithms(const CryptoSuite& suite)
    {
        {
            const auto err = this->handshake.configure(suite.dh_mode, suite.handshake_kdf, suite.handshake_hash);
            if (any(err))
            {
                throw Exception("Unable to configure handshake algorithms: ", HandshakeErrorSpec::to_string(err));
            }
        }

        {
            const auto err = this->session.configure(suite.nonce_mode, suite.session_mode);
            if (any(err))
            {
                throw Exception("Unable to configure session algorithms: ", HandshakeErrorSpec::to_string(err));
            }
        }
    }

    HandshakeError Algorithms::Session::configure(NonceMode nonce_mode, SessionMode session_mode)
    {
        switch (nonce_mode)
        {
        case(NonceMode::greater_than_last_rx):
            this->verify_nonce = &NonceFunctions::verify_greater_than_last;
            break;
        case(NonceMode::increment_last_rx):
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

    HandshakeError Algorithms::Handshake::configure(
        DHMode dh_mode,
        HandshakeKDF handshake_kdf,
        HandshakeHash handshake_hash
    )
    {
        switch (dh_mode)
        {
        case(DHMode::x25519):
            this->dh = &Crypto::dh_x25519;
            this->gen_keypair = &Crypto::gen_keypair_x25519;
            break;
        default:
            return HandshakeError::unsupported_dh_mode;
        }

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

    HandshakeError Algorithms::configure(const CryptoSpec& spec)
    {
        Algorithms algorithms;

        {
            const auto err = algorithms.handshake.configure(spec.dh_mode, spec.handshake_kdf, spec.handshake_hash);
            if (any(err)) return err;
        }

        {
            const auto err = algorithms.session.configure(spec.nonce_mode, spec.session_mode);
            if (any(err)) return err;
        }

        (*this) = algorithms;

        return HandshakeError::none;
    }

}


