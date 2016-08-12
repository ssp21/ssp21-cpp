

#include "catch.hpp"

#include "ssp21/Crypto.h"

#define SUITE(name) "Curve25519TestSuite - " name

using namespace ssp21;

void CheckKeys(KeyPair& kp, KeyType type)
{
	REQUIRE(kp.public_key.get_key_type() == type);
	REQUIRE(kp.private_key.get_key_type() == type);
}

TEST_CASE(SUITE("DH_X25519"))
{
	// derive two key pairs 
	KeyPair kp1;
	Crypto::gen_keypair_x25519(kp1);
	CheckKeys(kp1, KeyType::X25519);

	KeyPair kp2;
	Crypto::gen_keypair_x25519(kp2);
	CheckKeys(kp2, KeyType::X25519);

	std::error_code ec;

	// derive the shared secrets
	Key shared_secret1;
	Crypto::dh_x25519(kp2.private_key, kp1.public_key, shared_secret1, ec);
	REQUIRE(!ec);
	REQUIRE(shared_secret1.get_key_type() == KeyType::X25519);
	
	Key shared_secret2;
	Crypto::dh_x25519(kp1.private_key, kp2.public_key, shared_secret2, ec);
	REQUIRE(!ec);
	REQUIRE(shared_secret2.get_key_type() == KeyType::X25519);

	// compare the shared secrets
	REQUIRE(Crypto::secure_equals(shared_secret1.as_slice(), shared_secret2.as_slice()));
}

