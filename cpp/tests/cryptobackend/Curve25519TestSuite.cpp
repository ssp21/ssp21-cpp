

#include "catch.hpp"

#include "ssp21/crypto/Crypto.h"

#define SUITE(name) "Curve25519TestSuite - " name

using namespace ssp21;

void CheckKeys(KeyPair& kp, BufferType type)
{
    REQUIRE(kp.public_key.get_type() == type);
    REQUIRE(kp.private_key.get_type() == type);
}

TEST_CASE(SUITE("key pair generation and dh operations result in same shared key"))
{
    // derive two key pairs
    KeyPair kp1;
    Crypto::gen_keypair_x25519(kp1);
    CheckKeys(kp1, BufferType::x25519_key);

    KeyPair kp2;
    Crypto::gen_keypair_x25519(kp2);
    CheckKeys(kp2, BufferType::x25519_key);

    std::error_code ec;

    // derive the shared secrets
    DHOutput shared_secret1;
    Crypto::dh_x25519(kp2.private_key, kp1.public_key.as_slice().widen<uint32_t>(), shared_secret1, ec);
    REQUIRE(!ec);
    REQUIRE(shared_secret1.get_type() == BufferType::x25519_key);

    DHOutput shared_secret2;
    Crypto::dh_x25519(kp1.private_key, kp2.public_key.as_slice().widen<uint32_t>(), shared_secret2, ec);
    REQUIRE(!ec);
    REQUIRE(shared_secret2.get_type() == BufferType::x25519_key);

    // compare the shared secrets
    REQUIRE(Crypto::secure_equals(shared_secret1.as_slice(), shared_secret2.as_slice()));
}

TEST_CASE(SUITE("performing dh operation on null public key fails"))
{
    KeyPair kp1;
    Crypto::gen_keypair_x25519(kp1);

    PublicKey pub;
    pub.get_write_slice().set_all_to(0);
    pub.set_type(BufferType::x25519_key);

    std::error_code ec;

    // derive the shared secrets
    DHOutput shared_secret;
    Crypto::dh_x25519(kp1.private_key, pub.as_slice().widen<uint32_t>(), shared_secret, ec);
    REQUIRE(ec);
}
