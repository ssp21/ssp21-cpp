
#include "catch.hpp"

#include "ssp21/crypto/Chain.h"
#include "ssp21/crypto/gen/CertificateChain.h"
#include "ssp21/util/Exception.h"

#include "mocks/CryptoFixture.h"
#include "mocks/MockCertificateData.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

using namespace ssp21;
using namespace openpal;

HandshakeError test_chain(uint8_t anchor_signing_level, CertificateChain& chain, HandshakeError expected_result, std::initializer_list<CryptoAction> actions);

#define SUITE(name) "ChainVerificationTestSuite - " name

TEST_CASE(SUITE("successfully verifies single certificate"))
{
    const MockCertificateData endpoint(0, PublicKeyType::X25519);
    CertificateChain chain;
    REQUIRE(chain.certificates.push(endpoint.envelope));

    test_chain(1, chain, HandshakeError::none, { CryptoAction::verify_ed25519 });
}

TEST_CASE(SUITE("successfully verifies certificate chain"))
{
    const MockCertificateData intermediate(1, PublicKeyType::Ed25519);
    const MockCertificateData endpoint(0, PublicKeyType::X25519);

    CertificateChain chain;
    REQUIRE(chain.certificates.push(intermediate.envelope));
    REQUIRE(chain.certificates.push(endpoint.envelope));

    test_chain(2, chain, HandshakeError::none, { CryptoAction::verify_ed25519, CryptoAction::verify_ed25519 });
}

HandshakeError test_chain(uint8_t anchor_signing_level, CertificateChain& chain, HandshakeError expected_result, std::initializer_list<CryptoAction> expected_actions)
{
    CryptoFixture fix;
    const MockCertificateData anchor(anchor_signing_level, PublicKeyType::Ed25519);

    CertificateBody body;
    const auto err = Chain::verify(anchor.body, chain.certificates, body);

    if (err != expected_result)
    {
        throw Exception("expected ", HandshakeErrorSpec::to_string(expected_result), " but result was: ", HandshakeErrorSpec::to_string(err));
    }

    fix.expect(expected_actions);

    return err;
}
