

#include "ssp21/crypto/Algorithms.h"

#include "ssp21/crypto/Crypto.h"

namespace ssp21
{

Algorithms::Algorithms() :
	dh(&Crypto::dh_x25519),
	hkdf(&Crypto::hkdf_sha256),
	hash(&Crypto::hash_sha256),
	gen_keypair(&Crypto::gen_keypair_x25519),
	verify_nonce(NonceFunctions::default_verify())
{}

HandshakeError Algorithms::configure(DHMode dh_mode, HashMode hash_mode, NonceMode nonce_mode)
{
	Algorithms algorithms;

	switch (dh_mode)
	{
	case(DHMode::x25519):
		algorithms.dh = &Crypto::dh_x25519;
		algorithms.gen_keypair = &Crypto::gen_keypair_x25519;
		break;
	default:
		return HandshakeError::unsupported_dh_mode;
	}

	switch (hash_mode)
	{
	case(HashMode::sha256):
		algorithms.hash = &Crypto::hash_sha256;
		algorithms.hkdf = &Crypto::hkdf_sha256;
		break;
	default:
		return HandshakeError::unsupported_hash_mode;
	}

	switch (nonce_mode)
	{
	case(NonceMode::greater_than_last_rx):
		algorithms.verify_nonce = &NonceFunctions::verify_greater_than_last;
		break;
	case(NonceMode::increment_last_rx):
		algorithms.verify_nonce = &NonceFunctions::verify_strict_increment;
		break;
	default:
		return HandshakeError::unsupported_nonce_mode;
	}

	(*this) = algorithms;

	return HandshakeError::none;
}

}


