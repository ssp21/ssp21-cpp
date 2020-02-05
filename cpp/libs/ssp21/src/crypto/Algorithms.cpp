

#include "crypto/Algorithms.h"
#include "ssp21/util/Exception.h"

namespace ssp21 {

HandshakeError Algorithms::Session::configure(SessionNonceMode nonce_mode, SessionCryptoMode session_mode)
{
    switch (nonce_mode) {
    case (SessionNonceMode::greater_than_last_rx):
        this->verify_nonce = &NonceFunctions::verify_greater_than_last;
        break;
    case (SessionNonceMode::strict_increment):
        this->verify_nonce = &NonceFunctions::verify_strict_increment;
        break;
    default:
        return HandshakeError::unsupported_nonce_mode;
    }

    switch (session_mode) {
    case (SessionCryptoMode::hmac_sha256_16): {
        if (!Crypto::supports_sha256()) {
            return HandshakeError::unsupported_session_mode;
        }
        this->session_mode = SessionModes::hmac_sha_256_trunc16();
        break;
    }
    case (SessionCryptoMode::aes_256_gcm):
        if (!Crypto::supports_aes256_gcm()) {
            return HandshakeError::unsupported_session_mode;
        }
        this->session_mode = SessionModes::aes_256_gcm();
        break;
    default:
        return HandshakeError::unsupported_session_mode;
    }

    return HandshakeError::none;
}

HandshakeError Algorithms::Handshake::configure(
    HandshakeKDF handshake_kdf,
    HandshakeHash handshake_hash)
{
    switch (handshake_hash) {
    case (HandshakeHash::sha256):
        if (!Crypto::supports_sha256()) {
            return HandshakeError::unsupported_handshake_hash;
        }
        this->hash = &Crypto::hash_sha256;
        break;
    default:
        return HandshakeError::unsupported_handshake_hash;
    }

    switch (handshake_kdf) {
    case (HandshakeKDF::hkdf_sha256):
        if (!Crypto::supports_sha256()) {
            return HandshakeError::unsupported_handshake_kdf;
        }
        this->kdf = &Crypto::hkdf_sha256;
        break;
    default:
        return HandshakeError::unsupported_handshake_kdf;
    }

    return HandshakeError::none;
}

Algorithms::Common Algorithms::Common::get_or_throw(const CryptoSuite& suite)
{
    Algorithms::Common ret;
    const auto err = ret.configure(
        suite.handshake_hash,
        suite.handshake_kdf,
        suite.session_nonce_mode,
        suite.session_crypto_mode);

    if (any(err)) {
        throw Exception("unable to configure algorithms: ", HandshakeErrorSpec::to_string(err));
    }

    return ret;
}

HandshakeError Algorithms::Common::configure(HandshakeHash handshake_hash,
                                             HandshakeKDF handshake_kdf,
                                             SessionNonceMode session_nonce_mode,
                                             SessionCryptoMode session_crypto_mode)
{
    {
        const auto err = this->handshake.configure(handshake_kdf, handshake_hash);
        if (any(err)) {
            return err;
        }
    }
    {
        const auto err = this->session.configure(session_nonce_mode, session_crypto_mode);
        if (any(err)) {
            return err;
        }
    }

    return HandshakeError::none;
}

HandshakeError Algorithms::Common::configure(const CryptoSpec& spec)
{
    return this->configure(
        spec.handshake_hash,
        spec.handshake_kdf,
        spec.session_nonce_mode,
        spec.session_crypto_mode);
}

Algorithms::DH Algorithms::DH::get_or_throw(HandshakeEphemeral type)
{
    Algorithms::DH ret;
    const auto err = ret.configure(type);
    if (any(err)) {
        throw Exception("Unable to configure DH algorithms: ", HandshakeErrorSpec::to_string(err));
    }
    return ret;
}

HandshakeError Algorithms::DH::configure(HandshakeEphemeral type)
{
    switch (type) {
    case (HandshakeEphemeral::x25519):
        if (!Crypto::supports_x25519()) {
            return HandshakeError::unsupported_handshake_ephemeral;
        }
        this->dh = &Crypto::dh_x25519;
        this->gen_key_pair = &Crypto::gen_keypair_x25519;
        return HandshakeError::none;
    default:
        return HandshakeError::unsupported_handshake_ephemeral;
    }
}

}
