
#ifndef SSP21_HANDSHAKE_H
#define SSP21_HANDSHAKE_H

#include "ssp21/util/SequenceTypes.h"

#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/RequestHandshakeBegin.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/NonceFunctions.h"
#include "ssp21/crypto/Algorithms.h"
#include "ssp21/crypto/Session.h"

namespace ssp21
{

    /**
    * Class that holds all of the data during the handshake process
    */
    class Handshake : private openpal::Uncopyable
    {
    public:

		explicit Handshake(HandshakeMode mode, std::unique_ptr<Session> session);

        HandshakeError set_algorithms(const CryptoSpec& spec);

        // generates new ephemeral keys, resets all state, and returns a slice pointing
        // to the ephemeral public DH key
        seq32_t initialize();

        // reads session constraints from the message
        // sets h = hash(raw_msg)
        void begin_handshake(const RequestHandshakeBegin& msg, const seq32_t& raw_msg);

        // mix the input input the chaining key: ck = hash(ck | input)
        void mix_handshake_hash(const seq32_t& input);

        // derive the authentication key from the DH keys and the handshake_hash
        void initialize_pending_session(
            const seq32_t& message,
            const PrivateKey& priv_s_dh_key,
            const seq32_t& pub_e_dh_key,
            const seq32_t& pub_s_dh_key,
            std::error_code& ec
        );

        // derive the session keys and initialize the session
        void initialize_session(Session& session, const openpal::Timestamp& session_start) const;		

    private:

		inline bool is_responder() const
		{
			return mode == HandshakeMode::Responder;
		}

        // configures the handshake for initiator or responder mode
        const HandshakeMode mode;

        // session constraints used to initialize the session
        SessionConstraints constraints;

        // specific algorithms used to perform steps
        Algorithms algorithms;

        // running hash value of first 2 handshake messages (h)
        SymmetricKey handshake_hash;        

        // ephemeral keys
        KeyPair local_ephemeral_keys;

		// pending session 
		std::unique_ptr<Session> pending_session;
    };



}

#endif
