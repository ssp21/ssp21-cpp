
#ifndef SSP21_HANDSHAKE_H
#define SSP21_HANDSHAKE_H

#include "CryptoTypedefs.h"

#include "ssp21/gen/HandshakeError.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HashMode.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/NonceFunctions.h"
#include "ssp21/crypto/Algorithms.h"

namespace ssp21
{

    /**
    * Class that holds all of the data during the handshake process
    */
    class Handshake : private openpal::Uncopyable
    {
    public:

        Handshake(EntityId id) : id_(id)
        {}

        HandshakeError set_algorithms(const Algorithms::Config& config);

        /// generates new ephemeral keys, resets all state, and returns a slice pointing
        /// to the ephemeral public DH key
        openpal::RSlice initialize();

        /// calculate a new ck: ck = hash(input)
        void set_ck(const openpal::RSlice& input);

        /// mix the input input the chaining key: ck = hash(ck | input)
        void mix_ck(const openpal::RSlice& input);

        /// derive the authentication key from the DH keys and the handshake_hash_
        void derive_authentication_key(
            const openpal::RSlice& message,
            const PrivateKey& priv_s_dh_key,
            const openpal::RSlice& pub_e_dh_key,
            const openpal::RSlice& pub_s_dh_key,
            std::error_code& ec
        );

        /// derive the session keys
        void derive_session_keys(SessionKeys& keys) const;

        bool auth_handshake(const openpal::RSlice& mac) const;

        void calc_auth_handshake_reply_mac(HashOutput& output) const;

    private:

        /// configures the handshake for initiator or responder mode
        EntityId id_;

        /// specific algorithms used to perform steps
        Algorithms algorithms_;

        /// running hash value and chaining key after the derive_authentication_key(...) step
        SymmetricKey chaining_key_;

        /// authentication key derived during derive_authentication_key(...)
        SymmetricKey authentication_key_;

        /// ephemeral keys
        KeyPair local_ephemeral_keys_;
    };



}

#endif
