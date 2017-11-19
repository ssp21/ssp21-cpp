
#ifndef SSP21_IINITIATORHANDSHAKE_H
#define SSP21_IINITIATORHANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"
#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/gen/HandshakeMode.h"


namespace ssp21
{

    /**
    * Interface that defines what a concrete initiator handshake needs to do
    */
    class IInitiatorHandshake : private openpal::Uncopyable
    {

    public:

        /**
        * Generate a new ephemeral data for the RequestHandshakeBegin message. This is a public DH key or random nonce.
        * This will modify internal state inside the object, as it needs to be recorded for processing when the response is received.
        *
        * @return ephemeral data
        */
        virtual seq32_t generate_ephemeral_data() = 0;

        /**
        * Initialize the handshake hash and record the time at which the request transmission started
        */
        virtual void finalize_request_tx(const seq32_t& request_data, const openpal::Timestamp& now) = 0;

        virtual bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const InitiatorConfig::Params& params, const openpal::Timestamp& now, Session& session) = 0;

        virtual HandshakeMode get_handshake_mode() const = 0;

        virtual seq32_t get_mode_data() const = 0;

        virtual CryptoSuite get_crypto_suite() const = 0;

    };



}

#endif
