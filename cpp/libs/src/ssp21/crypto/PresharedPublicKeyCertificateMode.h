
#ifndef SSP21_PRESHAREDPUBLICKEYCERTIFICATEMODE_H
#define SSP21_PRESHAREDPUBLICKEYCERTIFICATEMODE_H

#include "ssp21/crypto/ICertificateMode.h"

#include "ssp21/crypto/BufferTypes.h"



#include <memory>

namespace ssp21
{

    /**
    * A public key validator that is configured with a pre-shared public key for other party
    */
    class PresharedPublicKeyCertificateMode final : public ICertificateMode
    {
    public:

        PresharedPublicKeyCertificateMode(
            const std::shared_ptr<const PublicKey>& remote_static_public_key
        ) :
            remote_static_public_key(remote_static_public_key)
        {}

        virtual seq32_t certificate_data() const override
        {
            return seq32_t::empty();
        }

        virtual CertificateMode mode() const override
        {
            return CertificateMode::preshared_keys;
        }

        virtual HandshakeError validate(CertificateMode mode, const seq32_t& certificate_data, seq32_t& public_key_output) override
        {
            if (mode != CertificateMode::preshared_keys) return HandshakeError::unsupported_certificate_mode;
            if (certificate_data.is_not_empty()) return HandshakeError::bad_message_format;

            public_key_output = this->remote_static_public_key->as_seq();

            return HandshakeError::none;
        }


    private:

        const std::shared_ptr<const PublicKey> remote_static_public_key;
    };



}

#endif
