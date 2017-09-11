
#ifndef SSP21_IINITIATORHANDSHAKE_H
#define SSP21_IINITIATORHANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/gen/CertificateMode.h"
#include "ssp21/crypto/Session.h"


namespace ssp21
{

    /**
    * Interface that the initiator uses to perform handshake
    */
    class IInitiatorHandshake : private openpal::Uncopyable
    {
    public:

        virtual seq32_t generate_ephemeral_data() = 0;

        virtual void begin_request_transmit(const seq32_t& data, const openpal::Timestamp& now) = 0;

        virtual bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const InitiatorConfig::Params& params, const openpal::Timestamp& now, Session& session) = 0;

        virtual CertificateMode get_certificate_mode() const = 0;

        virtual seq32_t get_mode_data() const = 0;

        virtual CryptoSuite get_crypto_suite() const = 0;

    };



}

#endif
