
#ifndef SSP21_ALGORITHMS_H
#define SSP21_ALGORITHMS_H

#include "CryptoTypedefs.h"

#include "ssp21/crypto/NonceFunctions.h"
#include "ssp21/crypto/SessionModes.h"

#include "ssp21/gen/DHMode.h"
#include "ssp21/gen/HashMode.h"
#include "ssp21/gen/NonceMode.h"
#include "ssp21/gen/HandshakeError.h"
#include "ssp21/gen/SessionMode.h"

namespace ssp21
{




/**
* Represents a complete set of algorithms for the handshake and the session
*/
struct Algorithms
{

public: 

	struct Config
	{
		Config(
			DHMode dh_mode,
			HashMode hash_mode,
			NonceMode nonce_mode,
			SessionMode session_mode) :
			dh_mode(dh_mode),
			hash_mode(hash_mode),
			nonce_mode(nonce_mode),
			session_mode(session_mode)
		{}

		DHMode dh_mode;
		HashMode hash_mode;
		NonceMode nonce_mode;
		SessionMode session_mode;
	};

	// default constructor initializes with default algorithms
	Algorithms();

	HandshakeError configure(const Config& config);

	dh_func_t dh;
    hkdf_func_t hkdf;
    hash_func_t hash;
    gen_keypair_func_t gen_keypair;
	verify_nonce_func_t verify_nonce;
	session_verify_t session_verify;
};


}

#endif
