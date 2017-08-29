
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

        InitiatorHandshake(openpal::Logger logger, const Algorithms& algorithms, const std::shared_ptr<ICertificateHandler>& cert_handler) :
            logger(logger),
            algorithms(algorithms),
            cert_handler(cert_handler)
        {}

        seq32_t generate_ephemerals();

        void init_handshake_hash(const seq32_t& data);

        bool initialize_session(const StaticKeys& keys, const ReplyHandshakeBegin& msg, const seq32_t& msg_bytes, const openpal::Timestamp& now, Session& session);

    private:

        seq32_t mix_handshake_hash(const seq32_t& data);

        openpal::Logger logger;

        // specific algorithms used to perform steps
        const Algorithms algorithms;
        const std::shared_ptr<ICertificateHandler> cert_handler;

        // ephemeral keys
        KeyPair local_ephemeral_keys;

        // running hash value of first 2 handshake messages (h)
        HashOutput handshake_hash;
    };



}

#endif
