

#include "catch.hpp"
#include "ssp21/Crypto.h"
#include <string.h>

#define SUITE(name) "Curve25519TestSuite - " name

using namespace ssp21;

void CheckKeyLengths(KeyPair& kp, uint32_t size)
{
	REQUIRE(kp.public_key.as_slice().Size() == size);
	REQUIRE(kp.private_key.as_slice().Size() == size);
}

TEST_CASE(SUITE("DH_X25519"))
{
	// derive two key pairs 
	KeyPair kp1;
	Crypto::gen_keypair_x25519(kp1);
	CheckKeyLengths(kp1, consts::X25519_KEY_LENGTH);

	KeyPair kp2;
	Crypto::gen_keypair_x25519(kp2);
	CheckKeyLengths(kp2, consts::X25519_KEY_LENGTH);

	std::error_code ec;

	// derive the shared secrets
	Key shared_secret1;
	Crypto::dh_x25519(kp2.private_key.as_slice(), kp1.public_key.as_slice(), shared_secret1, ec);
	REQUIRE(!ec);
	REQUIRE(shared_secret1.as_slice().Size() == consts::X25519_KEY_LENGTH);
	
	Key shared_secret2;
	Crypto::dh_x25519(kp1.private_key.as_slice(), kp2.public_key.as_slice(), shared_secret2, ec);
	REQUIRE(!ec);
	REQUIRE(shared_secret2.as_slice().Size() == consts::X25519_KEY_LENGTH);

	// compare the shared secrets
	REQUIRE(memcmp(shared_secret1.as_slice(), shared_secret2.as_slice(), consts::X25519_KEY_LENGTH) == 0); 
}

