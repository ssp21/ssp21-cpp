
#ifndef SSP21_PRESHAREDKEYCERTIFICATEHANDLER_H
#define SSP21_PRESHAREDKEYCERTIFICATEHANDLER_H

#include "ssp21/crypto/ICertificateHandler.h"

#include "ssp21/crypto/BufferTypes.h"

#include <memory>

namespace ssp21 {

/**
    * A public key validator that is configured with a pre-shared public key for other party
    */
class PresharedKeyCertificateHandler final : public ICertificateHandler {
public:
    PresharedKeyCertificateHandler(
        const std::shared_ptr<const PublicKey>& remote_static_public_key)
        : remote_static_public_key(remote_static_public_key)
    {
    }

    virtual seq32_t certificate_data() const override
    {
        return seq32_t::empty();
    }

    virtual HandshakeMode mode() const override
    {
        return HandshakeMode::preshared_public_keys;
    }

    virtual HandshakeError validate(HandshakeMode mode, const seq32_t& certificate_data, seq32_t& public_key_output) override
    {
        if (mode != HandshakeMode::preshared_public_keys)
            return HandshakeError::unsupported_handshake_mode;
        if (certificate_data.is_not_empty())
            return HandshakeError::bad_message_format;

        public_key_output = this->remote_static_public_key->as_seq();

        return HandshakeError::none;
    }

private:
    const std::shared_ptr<const PublicKey> remote_static_public_key;
};

}

#endif
