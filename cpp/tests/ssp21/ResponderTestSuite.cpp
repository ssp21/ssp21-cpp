
#include "catch.hpp"

#include "fixtures/ResponderFixture.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

// helper methods
void test_begin_handshake_success(ResponderFixture& fix);
void test_auth_handshake_success(ResponderFixture& fix);
void test_init_session_success(ResponderFixture& fix);
void test_handshake_error(ResponderFixture& fix, const std::string& request, HandshakeError expected_error, std::initializer_list<CryptoAction> actions);

// ---------- tests for handshake state idle -----------

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_BEGIN with REPLY_HANDSHAKE_BEGIN"))
{
    ResponderFixture fix;
    fix.responder.on_open();
    test_begin_handshake_success(fix);
}

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_AUTH with no_prior_handshake error"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    const auto request = hex::request_handshake_auth(hex::repeat(0xFF, consts::crypto::sha256_hash_output_length));
    test_handshake_error(fix, request, HandshakeError::no_prior_handshake_begin, {});
}

TEST_CASE(SUITE("responds to malformed handshake begin with bad_message_format"))
{
    ResponderFixture fix;
    fix.responder.on_open();
    test_handshake_error(fix, "00", HandshakeError::bad_message_format, {});
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

    test_handshake_error(fix, request, HandshakeError::unsupported_certificate_mode, {});
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

    test_handshake_error(fix, request, HandshakeError::bad_message_format, {});
}

// ---------- tests for handshake state wait for auth -----------

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_AUTH with REPLY_HANDSHAKE_AUTH"))
{
    ResponderFixture fix;
    fix.responder.on_open();

	test_init_session_success(fix);
}

TEST_CASE(SUITE("responds to auth request w/ invalid HMAC"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    test_begin_handshake_success(fix);

    const auto request = hex::request_handshake_auth(hex::repeat(0xEE, consts::crypto::sha256_hash_output_length));
    test_handshake_error(fix, request, HandshakeError::authentication_error, {CryptoAction::hmac_sha256, CryptoAction::secure_equals });
}

TEST_CASE(SUITE("responds to auth request with insufficient hmac size with authentication error"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    test_begin_handshake_success(fix);

    const auto request = hex::request_handshake_auth(hex::repeat(0xFF, (consts::crypto::sha256_hash_output_length - 1)));
    test_handshake_error(fix, request, HandshakeError::authentication_error, { CryptoAction::hmac_sha256, CryptoAction::secure_equals });
}

TEST_CASE(SUITE("handshake process can be repeated"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    for (int i = 0; i < 3; ++i)
    {
        test_begin_handshake_success(fix);
        test_auth_handshake_success(fix);
    }
}

TEST_CASE(SUITE("begin handshake can be repeated prior to auth handshake"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    test_begin_handshake_success(fix);
    test_begin_handshake_success(fix);
    test_begin_handshake_success(fix);
    test_auth_handshake_success(fix);
}

// ---------- tests for initialized session -----------

TEST_CASE(SUITE("can authenticate session data"))
{
	ResponderFixture fix;
	fix.responder.on_open();

	test_init_session_success(fix);

	const auto data_and_tag = hex::repeat(0xFF, 18);

	fix.lower.enqueue_message(Addresses(1, 10), hex::session_data(0, 0xFFFFFFFF, true, true, data_and_tag));
	fix.responder.on_rx_ready();
	REQUIRE(fix.upper.pop_rx_message() == "FF FF");
	
}

// ---------- helper method implementations -----------

void test_begin_handshake_success(ResponderFixture& fix)
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
    {
        CryptoAction::gen_keypair_x25519,	// generate local ephemeral key pair
        CryptoAction::hash_sha256,			// mix ck of received message
        CryptoAction::hash_sha256,          // mix ck of transmitted message
        CryptoAction::dh_x25519,		    // 3 DH operations to calculate ak and ck
        CryptoAction::dh_x25519,			//
        CryptoAction::dh_x25519,			//
        CryptoAction::hmac_sha256,			// 3 HMACs for the initial HKDF
        CryptoAction::hmac_sha256,			//
        CryptoAction::hmac_sha256			//
    });

    const auto reply = hex::reply_handshake_begin(hex::repeat(0xFF, consts::crypto::x25519_key_length));
    REQUIRE(fix.lower.pop_tx_message() == reply);
    fix.set_tx_ready();
}

void test_auth_handshake_success(ResponderFixture& fix)
{
    const auto mac_hex = hex::repeat(0xFF, consts::crypto::sha256_hash_output_length);

    fix.lower.enqueue_message(Addresses(1, 10), hex::request_handshake_auth(mac_hex));
    fix.responder.on_rx_ready();
    REQUIRE(fix.lower.pop_tx_message() == hex::request_handshake_auth(mac_hex));

    // expected order of crypto operations
    MockCryptoBackend::instance.expect(
    {
        CryptoAction::hmac_sha256,		// calculate expected value
        CryptoAction::secure_equals,	// compare MAC values
        CryptoAction::hash_sha256,		// mix the received message
        CryptoAction::hmac_sha256,		// calculate reply value
        CryptoAction::hash_sha256,		// mix the reply
        CryptoAction::hmac_sha256,		// 3 HMACs for the final HKDF to calculate session keys
        CryptoAction::hmac_sha256,		//
        CryptoAction::hmac_sha256		//
    });

    fix.set_tx_ready();
}

void test_init_session_success(ResponderFixture& fix)
{
	test_begin_handshake_success(fix);
	test_auth_handshake_success(fix);
}

void test_handshake_error(ResponderFixture& fix, const std::string& request, HandshakeError expected_error, std::initializer_list<CryptoAction> actions)
{
    fix.lower.enqueue_message(Addresses(1, 10), request);
    REQUIRE(fix.lower.num_rx_messages() == 1);
    fix.responder.on_rx_ready();
    REQUIRE(fix.lower.num_rx_messages() == 0);

    MockCryptoBackend::instance.expect(actions);

    REQUIRE(fix.lower.pop_tx_message() == hex::reply_handshake_error(expected_error));
    fix.set_tx_ready();
}


