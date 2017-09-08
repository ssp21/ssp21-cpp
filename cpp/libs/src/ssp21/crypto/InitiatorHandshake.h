
#ifndef SSP21_INITIATORHANDSHAKE_H
#define SSP21_INITIATORHANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/NonceFunctions.h"
#include "ssp21/crypto/Algorithms.h"
#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/StaticKeys.h"
#include "ssp21/crypto/ICertificateHandler.h"

namespace ssp21
{

    /**
    * Class that holds all of the data used during the handshake process
    */
    class InitiatorHandshake : private openpal::Uncopyable
    {
    public:

        InitiatorHandshake(openpal::Logger logger, StaticKeys static_keys, const CryptoSuite& crypto_suite, const std::shared_ptr<ICertificateHandler>& cert_handler) :
            logger(logger),
            static_keys(static_keys),
            crypto_suite(crypto_suite),
            algorithms(crypto_suite),
            cert_handler(cert_handler)
        {}

        seq32_t generate_ephemeral_data();

        void begin_request_transmit(const seq32_t& data, const openpal::Timestamp& now);

        bool initialize_session(const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const InitiatorConfig::Params& params, const openpal::Timestamp& now, Session& session);

        inline CertificateMode get_certificate_mode() const
        {
            return cert_handler->mode();
        }

        inline seq32_t get_mode_data() const
        {
            return cert_handler->certificate_data();
        }

        inline CryptoSuite get_crypto_suite() const
        {
            return this->crypto_suite;
        }

    private:

        seq32_t mix_handshake_hash(const seq32_t& data);

        openpal::Logger logger;

        const StaticKeys static_keys;

        // specific algorithms used to perform steps
        const CryptoSuite crypto_suite;
        const Algorithms algorithms;
        const std::shared_ptr<ICertificateHandler> cert_handler;

        // ephemeral keys
        KeyPair local_ephemeral_keys;

        // time that the request was transmitted
        openpal::Timestamp time_request_tx;

        // running hash value of first 2 handshake messages (h)
        HashOutput handshake_hash;
    };



}

#endif
