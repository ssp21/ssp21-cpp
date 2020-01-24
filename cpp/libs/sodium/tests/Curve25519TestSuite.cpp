

#include "catch.hpp"

#include "ssp21/crypto/Crypto.h"

#define SUITE(name) "Curve25519TestSuite - " name

using namespace ssp21;

void CheckKeys(KeyPair& kp, BufferLength length)
{
    REQUIRE(kp.public_key.get_length() == length);
    REQUIRE(kp.private_key.get_length() == length);
}

TEST_CASE(SUITE("key pair generation and dh operations result in same shared key"))
{
    // derive two key pairs
    KeyPair kp1;
    Crypto::gen_keypair_x25519(kp1);
    CheckKeys(kp1, BufferLength::length_32);

    KeyPair kp2;
    Crypto::gen_keypair_x25519(kp2);
    CheckKeys(kp2, BufferLength::length_32);

    std::error_code ec;

    // derive the shared secrets
    DHOutput shared_secret1;
    Crypto::dh_x25519(kp2.private_key, kp1.public_key.as_seq(), shared_secret1, ec);
    REQUIRE(!ec);
    REQUIRE(shared_secret1.get_length() == BufferLength::length_32);

    DHOutput shared_secret2;
    Crypto::dh_x25519(kp1.private_key, kp2.public_key.as_seq(), shared_secret2, ec);
    REQUIRE(!ec);
    REQUIRE(shared_secret2.get_length() == BufferLength::length_32);

    // compare the shared secrets
    REQUIRE(Crypto::secure_equals(shared_secret1.as_seq(), shared_secret2.as_seq()));
}

TEST_CASE(SUITE("performing dh operation on null public key fails"))
{
    KeyPair kp1;
    Crypto::gen_keypair_x25519(kp1);

    PublicKey pub;
    pub.as_wseq().set_all_to(0);
    pub.set_length(BufferLength::length_32);

    std::error_code ec;

    // derive the shared secrets
    DHOutput shared_secret;
    Crypto::dh_x25519(kp1.private_key, pub.as_seq(), shared_secret, ec);
    REQUIRE(ec);
}
