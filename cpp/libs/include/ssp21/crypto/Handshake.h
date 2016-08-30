
#ifndef SSP21_HANDSHAKE_H
#define SSP21_HANDSHAKE_H

#include "CryptoTypedefs.h"

#include "ssp21/gen/HandshakeError.h"
#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HashMode.h"

#include "ssp21/crypto/Crypto.h"
#include "ssp21/crypto/NonceFunctions.h"

namespace ssp21
{

    /**
    * Class that holds all of the data during the handshake process
    */
    class Handshake : private openpal::Uncopyable
    {
    public:

        struct Algorithms
        {

        public:            

			// default algorithms
			Algorithms() :
				dh(&Crypto::dh_x25519),
				hkdf(&Crypto::hkdf_sha256),
				hash(&Crypto::hash_sha256),
				gen_keypair(&Crypto::gen_keypair_x25519),
				verify_nonce(NonceFunctions::default_verify())
			{}			

			dh_func_t dh;
            hkdf_func_t hkdf;
            hash_func_t hash;
            gen_keypair_func_t gen_keypair;
			verify_nonce_func_t verify_nonce;
        };

		Handshake() {}

		HandshakeError set_algorithms(DHMode dh_mode, HashMode hash_mode, NonceMode nonce_mode);

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

        /// return a slice pointing to the authentication key
        openpal::RSlice get_auth_key() const;		

        /// derive the session keys
        void derive_session_keys(SymmetricKey& rx_key, SymmetricKey& tx_key) const;

    private:

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
