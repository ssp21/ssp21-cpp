
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
        );

        virtual CertificateMode mode() const override
        {
            return CertificateMode::preshared_keys;
        }

        virtual seq32_t certificate_data() const override
        {
            return seq32_t::empty();
        }

        virtual HandshakeError validate(const ssp21::seq32_t& certificates, seq32_t& public_key_output) override;


    private:

        const std::shared_ptr<const PublicKey> remote_static_public_key;
    };



}

#endif
