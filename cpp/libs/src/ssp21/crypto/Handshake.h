
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
    * Class that holds all of the data used during the handshake process
    */
    class Handshake : private openpal::Uncopyable
    {
    public:

		Handshake(const Algorithms::Handshake& algorithms) : algorithms(algorithms)
		{}

		seq32_t generate_ephemerals();

		void init_handshake_hash(const seq32_t& data);

		seq32_t mix_handshake_hash(const seq32_t& data);

	private:        

        // specific algorithms used to perform steps
        const Algorithms::Handshake algorithms;        

        // ephemeral keys
		KeyPair local_ephemeral_keys;

		// running hash value of first 2 handshake messages (h)
		HashOutput handshake_hash;
    };



}

#endif
