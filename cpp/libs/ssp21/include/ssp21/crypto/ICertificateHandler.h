
#ifndef SSP21_ICERTIFICATEHANDLER_H
#define SSP21_ICERTIFICATEHANDLER_H

#include "ser4cpp/util/Uncopyable.h"

#include "ssp21/crypto/BufferTypes.h"
#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/HandshakeMode.h"

#include "ssp21/util/ICollection.h"
#include "ssp21/util/SecureDynamicBuffer.h"
#include "ssp21/util/SequenceTypes.h"

#include <memory>

namespace ssp21 {

/**
    * Interface used to verify certificate data.
    *
    * Implementations could be for preshared public keys or retrieved from a certificate chain
    * authenticated by a trust anchor.
    */
class ICertificateHandler : private ser4cpp::Uncopyable {
public:
    virtual ~ICertificateHandler() {}

    /**
        * The certificate data to present to the other party during the handshake
        */
    virtual seq32_t certificate_data() const = 0;

    /**
        * Initiator side mode query
        */
    virtual HandshakeMode mode() const = 0;

    /**
        *  Given a particular certificate mode, validate the certificate data payload, and return a seq_t pointing to the validated public key
        */
    virtual HandshakeError validate(HandshakeMode mode, const seq32_t& certificate_data, seq32_t& public_key_output) = 0;

    /**
        *  Given a particular certificate mode, validate the certificate data payload, and return a seq_t pointing to the validated public key
        */
    HandshakeError validate(const seq32_t& certificate_data, seq32_t& public_key_output)
    {
        return this->validate(this->mode(), certificate_data, public_key_output);
    }

    // ---- factory functions for various implementations ----

    static std::shared_ptr<ICertificateHandler> preshared_key(const std::shared_ptr<const PublicKey>& remote_static_public_key);

    static std::shared_ptr<ICertificateHandler> certificates(
        const std::shared_ptr<ssp21::SecureDynamicBuffer>& anchor_cert_file_data,
        const std::shared_ptr<ssp21::SecureDynamicBuffer>& presented_chain_file_data);
};

}

#endif
