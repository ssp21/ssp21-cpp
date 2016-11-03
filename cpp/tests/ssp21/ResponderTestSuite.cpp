
#include "fixtures/ResponderFixture.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_BEGIN with REPLY_HANDSHAKE_BEGIN"))
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
}

TEST_CASE(SUITE("responds to malformed handshake begin with bad_message_format"))
{
    ResponderFixture fix;
    fix.responder.on_open();

    fix.test_handshake_error("00", HandshakeError::bad_message_format);
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

    fix.test_handshake_error(request, HandshakeError::unsupported_certificate_mode);
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

    fix.test_handshake_error(request, HandshakeError::bad_message_format);
}
