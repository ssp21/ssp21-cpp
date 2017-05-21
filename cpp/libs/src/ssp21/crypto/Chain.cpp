

#include "ssp21/crypto/Chain.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/Crypto.h"

namespace ssp21
{

    HandshakeError Chain::verify(const CertificateBody& anchor, const ICollection<CertificateEnvelope>& certificates, CertificateBody& result)
    {
        if (certificates.count() == 0) return HandshakeError::bad_certificate_chain;

        auto parent = &anchor;

        for (uint32_t i = 0; i < certificates.count(); ++i)
        {
            const auto child_env = certificates.get(i);

            const auto err = verify_pair(*parent, *child_env, result);
            if (any(err)) return err;

            parent = &result; // prepare for next iteration
        }

		// terminal certificate must have signing level == 0
		if (result.signing_level != 0) return HandshakeError::bad_certificate_chain;

		// terminal certificate must have a DH key
		if (!is_dh_key(result.public_key_type)) return HandshakeError::bad_certificate_chain;

		return HandshakeError::none;
    }

    HandshakeError Chain::verify_pair(const CertificateBody& parent, const CertificateEnvelope& child_envelope, CertificateBody& child)
    {
        HashOutput issuer_hash;
        Crypto::hash_sha256({ parent.public_key }, issuer_hash);
        auto calculated_issuer_id = issuer_hash.as_seq().take<uint8_t>(16); // TODO - make this a constant

        if (!child_envelope.issuer_id.equals(calculated_issuer_id)) return HandshakeError::bad_certificate_chain;

        const auto dsa_info = try_get_dsa_info(parent.public_key_type);

        if (dsa_info.verify == nullptr) return HandshakeError::bad_certificate_chain;

        if (dsa_info.signature_length != parent.public_key.length()) return HandshakeError::bad_certificate_chain;

        if (!dsa_info.verify(child_envelope.certificate_body, child_envelope.signature, parent.public_key)) return HandshakeError::authentication_error;

        if (any(child.read_all(child_envelope.certificate_body))) return HandshakeError::bad_certificate_format;

        if (child.valid_after < parent.valid_after) return HandshakeError::bad_certificate_chain;

        if (child.valid_before > parent.valid_before) return HandshakeError::bad_certificate_chain;

        if(child.signing_level >= parent.signing_level) return HandshakeError::bad_certificate_chain;

        return HandshakeError::none;
    }

    Chain::DSAInfo Chain::try_get_dsa_info(PublicKeyType type)
    {
        switch (type)
        {
        case(PublicKeyType::Ed25519):
            return DSAInfo { Crypto::verify_ed25519, consts::crypto::ed25519_signature_length };
        default:
            return DSAInfo { nullptr, 0 };
        }
    }

    bool Chain::is_dh_key(PublicKeyType type)
    {
        switch (type)
        {
        case(PublicKeyType::X25519):
            return true;
        default:
            return false;
        }
    }

}
