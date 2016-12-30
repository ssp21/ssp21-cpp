
#ifndef SSP21_HANDSHAKE_H
#define SSP21_HANDSHAKE_H

#include "CryptoTypedefs.h"

#include "ssp21/SequenceTypes.h"

#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/gen/DHMode.h"
#include "ssp21/crypto/gen/HandshakeHash.h"

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

        explicit Handshake(EntityId id) : id(id)
        {}

        HandshakeError set_algorithms(const CryptoSpec& spec);

        /// generates new ephemeral keys, resets all state, and returns a slice pointing
        /// to the ephemeral public DH key
        seq8_t initialize();

        /// calculate a new ck: ck = hash(input)
        void set_ck(const seq32_t& input);

        /// mix the input input the chaining key: ck = hash(ck | input)
        void mix_ck(const seq32_t& input);

        /// derive the authentication key from the DH keys and the handshake_hash
        void derive_authentication_key(
            const seq32_t& message,
            const PrivateKey& priv_s_dh_key,
            const seq8_t& pub_e_dh_key,
            const seq8_t& pub_s_dh_key,
            std::error_code& ec
        );

        /// derive the session keys and initialize the session
        void initialize_session(Session& session, const openpal::Timestamp& session_init_time) const;

        bool auth_handshake(const seq8_t& mac) const;

        void calc_auth_handshake_reply_mac(HashOutput& output) const;

    private:

        /// configures the handshake for initiator or responder mode
        EntityId id;

        /// specific algorithms used to perform steps
        Algorithms algorithms;

        /// running hash value and chaining key after the derive_authentication_key(...) step
        SymmetricKey chaining_key;

        /// authentication key derived during derive_authentication_key(...)
        SymmetricKey authentication_key;

        /// ephemeral keys
        KeyPair local_ephemeral_keys;
    };



}

#endif
