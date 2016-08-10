

#include "catch.hpp"
#include "ssp21/Crypto.h"
#include <string.h>

#define SUITE(name) "Curve25519TestSuite - " name

using namespace ssp21;

void CheckKeyLengths(KeyPair& kp, uint32_t size)
{
	REQUIRE(kp.publicKey.GetKey().Size() == size);
	REQUIRE(kp.privateKey.GetKey().Size() == size);		
}

TEST_CASE(SUITE("DH_X25519"))
{
	// derive two key pairs 
	KeyPair kp1;
	Crypto::GenKeyPair_X25519(kp1);
	CheckKeyLengths(kp1, consts::X25519_KEY_LENGTH);

	KeyPair kp2;
	Crypto::GenKeyPair_X25519(kp2);
	CheckKeyLengths(kp2, consts::X25519_KEY_LENGTH);

	std::error_code ec;

	// derive the shared secrets
	Key shared_secret1;
	Crypto::DH_X25519(kp2.privateKey.GetKey(), kp1.publicKey.GetKey(), shared_secret1, ec);
	REQUIRE(!ec);
	REQUIRE(shared_secret1.GetKey().Size() == consts::X25519_KEY_LENGTH);
	
	Key shared_secret2;
	Crypto::DH_X25519(kp1.privateKey.GetKey(), kp2.publicKey.GetKey(), shared_secret2, ec);
	REQUIRE(!ec);
	REQUIRE(shared_secret2.GetKey().Size() == consts::X25519_KEY_LENGTH);

	// compare the shared secrets
	REQUIRE(memcmp(shared_secret1.GetKey(), shared_secret2.GetKey(), consts::X25519_KEY_LENGTH) == 0); 
}

