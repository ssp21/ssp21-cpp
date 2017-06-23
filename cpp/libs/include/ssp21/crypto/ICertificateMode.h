
#ifndef SSP21_ICERTIFICATEMODE_H
#define SSP21_ICERTIFICATEMODE_H

#include <openpal/util/Uncopyable.h>

#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/CertificateMode.h"
#include "ssp21/crypto/BufferTypes.h"

#include "ssp21/util/SequenceTypes.h"
#include "ssp21/util/ICollection.h"

#include <memory>

namespace ssp21
{

    /**
    * Interface used to verify the certificate mode.
    *
    * Implementations could be for preshared public keys or retrieved from a certificate chain
    * authenticated by a trust anchor.
    */
    class ICertificateMode : private openpal::Uncopyable
    {
    public:

        virtual ~ICertificateMode() {}

        /**
        * The certificate data to present to the other party during the handshake
        */
        virtual seq32_t certificate_data() const = 0;

        /**
        * Initiator side mode query
        */
        virtual CertificateMode mode() const = 0;

        /**
        *  Given a particular certificate mode, validate the certificate data payload, and return a seq_t pointing to the validated public key
        */
        virtual HandshakeError validate(CertificateMode mode, const seq32_t& certificate_data, seq32_t& public_key_output) = 0;

        /**
        *  Given a particular certificate mode, validate the certificate data payload, and return a seq_t pointing to the validated public key
        */
        HandshakeError validate(const seq32_t& certificate_data, seq32_t& public_key_output)
        {
            return this->validate(this->mode(), certificate_data, public_key_output);
        }

        // ---- factory functions for various implementations ----

        static std::shared_ptr<ICertificateMode> preshared_key(const std::shared_ptr<const PublicKey>& remote_static_public_key);
    };



}

#endif
