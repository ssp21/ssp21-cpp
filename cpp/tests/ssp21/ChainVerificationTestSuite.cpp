
#include "catch.hpp"

#include "ssp21/crypto/Chain.h"
#include "ssp21/crypto/gen/CertificateChain.h"

#include "mocks/CryptoFixture.h"
#include "mocks/MockCertificateData.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

using namespace ssp21;
using namespace openpal;

#define SUITE(name) "ChainVerificationTestSuite - " name

TEST_CASE(SUITE("successfully verifies single certificate"))
{
    CryptoFixture fix;

    const Hex anchor_public_key(repeat_hex(0xFF, consts::crypto::x25519_key_length));

    const MockCertificateData anchor(1, PublicKeyType::Ed25519);
    const MockCertificateData endpoint(0, PublicKeyType::X25519);

    CertificateChain chain;
    REQUIRE(chain.certificates.push(endpoint.envelope));

    CertificateBody body;
    const auto err = Chain::verify(anchor.body, chain.certificates, body);
    REQUIRE(err == HandshakeError::none);
}

