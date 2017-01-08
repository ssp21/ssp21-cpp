
#include "catch.hpp"

#include "fixtures/CryptoLayerFixture.h"

#define SUITE(name) "InitiatorTestSuite - " name

using namespace ssp21;
using namespace openpal;

// ---------- tests for handshake state idle -----------

TEST_CASE(SUITE("construction"))
{
    InitiatorFixture fix;
}

TEST_CASE(SUITE("transmits REQUEST_HANDSHAKE_BEGIN when opened"))
{
    InitiatorFixture fix;
    REQUIRE(fix.lower.num_tx_messages() == 0);
    fix.initiator.on_open();
    REQUIRE(fix.lower.num_tx_messages() == 1);

    const auto expected = hex::request_handshake_begin(
                              0,
                              NonceMode::increment_last_rx,
                              DHMode::x25519,
                              HandshakeHash::sha256,
                              HandshakeKDF::hkdf_sha256,
                              HandshakeMAC::hmac_sha256,
                              SessionMode::hmac_sha256_16,
                              CertificateMode::preshared_keys,
                              hex::repeat(0xFF, 32)
                          );

    REQUIRE(fix.lower.pop_tx_message() == expected);
    REQUIRE(fix.exe->num_pending_timers() == 1);
}

