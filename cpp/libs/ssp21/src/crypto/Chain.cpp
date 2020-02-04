

#include "ssp21/crypto/Chain.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/Crypto.h"

namespace ssp21 {

HandshakeError Chain::verify(const CertificateBody& anchor, const ICollection<CertificateEnvelope>& certificates, CertificateBody& result)
{
    if (certificates.count() == 0) {
        return HandshakeError::bad_certificate_chain;
    }

    CertificateBody parent = anchor;

    for (uint32_t i = 0; i < certificates.count(); ++i) {
        const auto child_env = certificates.get(i);

        CertificateBody output;
        const auto err = verify_pair(parent, *child_env, output);
        if (any(err))
            return err;

        parent = output;
    }

    // terminal certificate must have signing level == 0
    if (parent.signing_level != 0) {
        return HandshakeError::bad_certificate_chain;
    }

    // terminal certificate must have a DH key
    if (!is_dh_key(parent.public_key_type)) {
        return HandshakeError::bad_certificate_chain;
    }

    result = parent;

    return HandshakeError::none;
}

HandshakeError Chain::verify_pair(const CertificateBody& parent, const CertificateEnvelope& child_envelope, CertificateBody& child)
{
    const auto dsa_info = try_get_dsa_info(parent.public_key_type);

    if (dsa_info.verify == nullptr)
        return HandshakeError::bad_certificate_chain;

    if (dsa_info.signature_length != child_envelope.signature.length()) {
        return HandshakeError::bad_certificate_chain;
    }

    if (!dsa_info.verify(child_envelope.certificate_body, child_envelope.signature, parent.public_key)) {
        return HandshakeError::authentication_error;
    }

    if (any(child.read_all(child_envelope.certificate_body))) {
        return HandshakeError::bad_certificate_format;
    }

    if (child.valid_after < parent.valid_after) {
        return HandshakeError::bad_certificate_chain;
    }

    if (child.valid_before > parent.valid_before) {
        return HandshakeError::bad_certificate_chain;
    }

    if (child.signing_level >= parent.signing_level) {
        return HandshakeError::bad_certificate_chain;
    }

    return HandshakeError::none;
}

Chain::DSAInfo Chain::try_get_dsa_info(PublicKeyType type)
{
    switch (type) {
    case (PublicKeyType::Ed25519): {
        if (Crypto::supports_ed25519()) {
            return DSAInfo{ Crypto::verify_ed25519, consts::crypto::ed25519_signature_length };
        } else {
            return DSAInfo{ nullptr, 0 };
        }
    }
    default:
        return DSAInfo{ nullptr, 0 };
    }
}

bool Chain::is_dh_key(PublicKeyType type)
{
    switch (type) {
    case (PublicKeyType::X25519):
        return true;
    default:
        return false;
    }
}

}
