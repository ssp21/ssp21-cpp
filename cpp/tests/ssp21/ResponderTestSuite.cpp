
#include "catch.hpp"

#include "fixtures/ResponderFixture.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

// helper methods
void test_begin_handshake(ResponderFixture& fix);
void test_auth_handshake(ResponderFixture& fix);
void test_handshake_error(ResponderFixture& fix, const std::string& request, HandshakeError expected_error);

// ---------- tests for handshake state idle -----------

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_BEGIN with REPLY_HANDSHAKE_BEGIN"))
{
    ResponderFixture fix;
    fix.responder.on_open();
    test_begin_handshake(fix);
}

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_AUTH with no_prior_handshake error"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    const auto request = hex::request_handshake_auth(hex::repeat(0xFF, consts::crypto::sha256_hash_output_length));
    test_handshake_error(fix, request, HandshakeError::no_prior_handshake_begin);
}

TEST_CASE(SUITE("responds to malformed handshake begin with bad_message_format"))
{
    ResponderFixture fix;
    fix.responder.on_open();
    test_handshake_error(fix, "00", HandshakeError::bad_message_format);
}

TEST_CASE(SUITE("responds to m2m certificate mode with unsupported_certificate_mode error"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    const auto request = hex::request_handshake_begin(
                             0,
                             NonceMode::increment_last_rx,
                             DHMode::x25519,
                             HashMode::sha256,
                             SessionMode::hmac_sha256_16,
                             CertificateMode::m2m,
                             hex::repeat(0xFF, consts::crypto::x25519_key_length)
                         );

    test_handshake_error(fix, request, HandshakeError::unsupported_certificate_mode);
}

TEST_CASE(SUITE("responds to invalid key length with bad_message_format"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    const auto request = hex::request_handshake_begin(
                             0,
                             NonceMode::increment_last_rx,
                             DHMode::x25519,
                             HashMode::sha256,
                             SessionMode::hmac_sha256_16,
                             CertificateMode::preshared_keys,
                             hex::repeat(0xFF, (consts::crypto::x25519_key_length - 1))
                         );

    test_handshake_error(fix, request, HandshakeError::bad_message_format);
}

// ---------- tests for handshake state wait for auth -----------

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_AUTH with REPLY_HANDSHAKE_AUTH"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    test_begin_handshake(fix);
    test_auth_handshake(fix);
}

void test_begin_handshake(ResponderFixture& fix)
{
    const auto request = hex::request_handshake_begin(
                             0,
                             NonceMode::increment_last_rx,
                             DHMode::x25519,
                             HashMode::sha256,
                             SessionMode::hmac_sha256_16,
                             CertificateMode::preshared_keys,
                             hex::repeat(0xFF, consts::crypto::x25519_key_length)
                         );

    fix.lower.enqueue_message(Addresses(1, 10), request);
    fix.responder.on_rx_ready();

    // expected order of crypto operations
    MockCryptoBackend::instance.expect(
        CryptoActions::gen_keypair_x25519,
        CryptoActions::hash_sha256,
        CryptoActions::hash_sha256,
        CryptoActions::dh_x25519,
        CryptoActions::dh_x25519,
        CryptoActions::dh_x25519,
        CryptoActions::hmac_sha256,
        CryptoActions::hmac_sha256,
        CryptoActions::hmac_sha256
    );

    const auto reply = hex::reply_handshake_begin(hex::repeat(0xFF, consts::crypto::x25519_key_length));
    REQUIRE(fix.lower.pop_tx_message() == reply);
    fix.set_tx_ready();
}

void test_auth_handshake(ResponderFixture& fix)
{
    const auto mac_hex = hex::repeat(0xFF, consts::crypto::sha256_hash_output_length);
    fix.lower.enqueue_message(Addresses(1, 10), hex::request_handshake_auth(mac_hex));
    fix.responder.on_rx_ready();
    REQUIRE(fix.lower.pop_tx_message() == hex::request_handshake_auth(mac_hex));




    fix.set_tx_ready();
}

void test_handshake_error(ResponderFixture& fix, const std::string& request, HandshakeError expected_error)
{
    fix.lower.enqueue_message(Addresses(1, 10), request);
    REQUIRE(fix.lower.num_rx_messages() == 1);
    fix.responder.on_rx_ready();
    REQUIRE(fix.lower.num_rx_messages() == 0);
    REQUIRE(MockCryptoBackend::instance.empty_actions());
    REQUIRE(fix.lower.pop_tx_message() == hex::reply_handshake_error(expected_error));
    fix.set_tx_ready();
}


