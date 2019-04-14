
#ifndef SSP21_ALGORITHMSET_H
#define SSP21_ALGORITHMSET_H

#include "crypto/Algorithms.h"

#include "ssp21/crypto/gen/HandshakeError.h"
#include "ssp21/crypto/CryptoSuite.h"
#include "crypto/gen/CryptoSpec.h"

#include "ssp21/util/Exception.h"

namespace ssp21
{
    /**
    * Represents a complete set of algorithms for the handshake and the session
    */
    template <class HandshakeAlgorithmsType>
    struct AlgorithmSet
    {

    public:

        // default constructor initializes with default algorithms
        AlgorithmSet() = default;

        // configure the algorithms from a CryptoSuite
        HandshakeError configure(const CryptoSuite& suite)
        {
            AlgorithmSet algorithms;

            {
                const HandshakeError err = algorithms.handshake.configure(suite.handshake_ephemeral, suite.handshake_kdf, suite.handshake_hash);
                if (any(err)) return err;
            }

            {
                const HandshakeError err = algorithms.session.configure(suite.nonce_mode, suite.session_mode);
                if (any(err)) return err;
            }

            (*this) = algorithms;

            return HandshakeError::none;
        }

        HandshakeError configure(const CryptoSpec& spec)
        {
            return this->configure(
                       CryptoSuite(                           
                           spec.handshake_ephemeral,
                           spec.handshake_hash,
                           spec.handshake_kdf,
						   spec.session_nonce_mode,
                           spec.session_mode
                       )
                   );
        }

        // construct the algorithms
        AlgorithmSet(const CryptoSuite& suite)
        {
            HandshakeError err = this->configure(suite);
            if (any(err))
            {
                throw Exception("Unable to configure algorithms: ", HandshakeErrorSpec::to_string(err));
            }
        }

        // handshake algorithms
        HandshakeAlgorithmsType handshake;

        // session algorithms
        Algorithms::Session session;
    };

    using public_key_algorithms_t = AlgorithmSet<Algorithms::PublicKeyHandshake>;
    using shared_secret_algorithms_t = AlgorithmSet<Algorithms::SharedSecretHandshake>;

}

#endif
