
#include "catch.hpp"

#include "fixtures/CryptoLayerFixture.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

// helper methods
void test_begin_handshake_success(ResponderFixture& fix, uint16_t max_nonce = consts::crypto::initiator::default_max_nonce, uint32_t max_session_time = consts::crypto::initiator::default_max_session_time_ms);
void test_auth_handshake_success(ResponderFixture& fix, const std::string& payload);
void test_init_session_success(ResponderFixture& fix, uint16_t max_nonce = consts::crypto::initiator::default_max_nonce, uint32_t max_session_time = consts::crypto::initiator::default_max_session_time_ms);
void test_handshake_error(ResponderFixture& fix, const std::string& request, HandshakeError expected_error, std::initializer_list<CryptoAction> actions);

// ---------- tests for handshake state idle -----------

TEST_CASE(SUITE("responds to REQUEST_HANDSHAKE_BEGIN with REPLY_HANDSHAKE_BEGIN"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();
    test_begin_handshake_success(fix);
}

TEST_CASE(SUITE("responds to session auth with no_prior_handshake error"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    const auto request = hex::session_data(0, 0xFFFFFFFF, "", hex::repeat(0xFF, 16));
    test_handshake_error(fix, request, HandshakeError::no_prior_handshake_begin, {});
}

TEST_CASE(SUITE("responds to malformed handshake begin with bad_message_format"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();
    test_handshake_error(fix, "00", HandshakeError::bad_message_format, {});
}

TEST_CASE(SUITE("responds to invalid key length with bad_message_format"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    const auto request = hex::request_handshake_begin(
        0,
        SessionNonceMode::strict_increment,
        HandshakeEphemeral::x25519,
        HandshakeHash::sha256,
        HandshakeKDF::hkdf_sha256,
        SessionCryptoMode::hmac_sha256_16,
        consts::crypto::initiator::default_max_nonce,
        consts::crypto::initiator::default_max_session_time_ms,
        HandshakeMode::public_keys,
        hex::repeat(0xFF, (consts::crypto::x25519_key_length - 1)));

    test_handshake_error(fix, request, HandshakeError::bad_message_format, {});
}

TEST_CASE(SUITE("ignores user data without a session"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    const auto request = hex::session_data(1, 0, "CA FE", hex::repeat(0xFF, 16));
    fix.lower.enqueue_message(request);

    REQUIRE(fix.upper.is_empty());
}

// ---------- tests for handshake state wait for auth -----------

TEST_CASE(SUITE("responds to auth session with auth session"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    REQUIRE_FALSE(fix.upper.get_is_open());
    test_init_session_success(fix);
    REQUIRE(fix.upper.get_is_open());
}

TEST_CASE(SUITE("ignores auth session w/ invalid HMAC value"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_begin_handshake_success(fix);

    const auto request = hex::session_data(0, 0xFFFFFF, "", hex::repeat(0xEE, consts::crypto::trunc16));
    fix.lower.enqueue_message(request);
    fix.expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });
    REQUIRE(fix.lower.num_tx_messages() == 0);
}

TEST_CASE(SUITE("ignores auth session w/ invalid HMAC size"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_begin_handshake_success(fix);

    const auto request = hex::session_data(0, 0xFFFFFF, "", hex::repeat(0xFF, consts::crypto::trunc16 - 1));
    fix.lower.enqueue_message(request);
    fix.expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });
    REQUIRE(fix.lower.num_tx_messages() == 0);
}

TEST_CASE(SUITE("handshake process can be repeated"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    for (int i = 0; i < 3; ++i) {
        test_begin_handshake_success(fix);
        test_auth_handshake_success(fix, "");
    }
}

TEST_CASE(SUITE("begin handshake can be repeated prior to auth handshake"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_begin_handshake_success(fix);
    test_begin_handshake_success(fix);
    test_begin_handshake_success(fix);
    test_auth_handshake_success(fix, "");
}

// ---------- rx tests for initialized session -----------

TEST_CASE(SUITE("closing the responder closes the upper layer"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_init_session_success(fix);
    REQUIRE(fix.upper.get_is_open());
    fix.responder.on_lower_close();
    REQUIRE_FALSE(fix.upper.get_is_open());
}

TEST_CASE(SUITE("auth fails if insufficient data for tag"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_init_session_success(fix);

    const auto short_tag = hex::repeat(0xFF, ssp21::consts::crypto::trunc16 - 1);
    fix.lower.enqueue_message(hex::session_data(1, 0xFFFFFFFF, "AA", short_tag));

    REQUIRE(fix.responder.get_statistics().num_auth_fail == 1);
    REQUIRE(fix.upper.is_empty());
}

TEST_CASE(SUITE("auth fails if TTL expired"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_init_session_success(fix);

    fix.exe->advance_time(std::chrono::milliseconds(3));

    const auto tag = hex::repeat(0xFF, ssp21::consts::crypto::trunc16);
    fix.lower.enqueue_message(hex::session_data(1, 2, "AA", tag)); // session TTL of 2

    REQUIRE(fix.responder.get_statistics().num_ttl_expiration == 1);
    REQUIRE(fix.upper.is_empty());
}

TEST_CASE(SUITE("fails on empty user data"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_init_session_success(fix);

    const auto tag = hex::repeat(0xFF, ssp21::consts::crypto::trunc16);

    fix.lower.enqueue_message(hex::session_data(1, 0xFFFFFFFF, "", tag));

    REQUIRE(fix.responder.get_statistics().num_auth_fail == 1);
    REQUIRE(fix.upper.is_empty());
}

TEST_CASE(SUITE("can authenticate session data"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_init_session_success(fix);

    const auto data = "01";
    const auto tag = hex::repeat(0xFF, ssp21::consts::crypto::trunc16);

    fix.lower.enqueue_message(hex::session_data(1, 0, data, tag));

    REQUIRE(fix.responder.get_statistics().num_success == 2); // initial auth + this message
    REQUIRE(fix.upper.pop_rx_message() == data);
}

TEST_CASE(SUITE("can authenticate multiple messages"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();

    test_init_session_success(fix);

    for (uint8_t i = 0; i < 3; ++i) {
        const auto data = HexConversions::to_hex(&i, 1);
        const auto tag = hex::repeat(0xFF, ssp21::consts::crypto::trunc16);

        fix.lower.enqueue_message(hex::session_data(i + 1, 0, data, tag));

        REQUIRE(fix.responder.get_statistics().num_success == (i + 2));
        REQUIRE(fix.upper.pop_rx_message() == data);
    }
}

// ---------- tx tests for initialized session -----------

TEST_CASE(SUITE("won't transmit if offline"))
{
    ResponderFixture fix;
    auto msg = HexConversions::from_hex("");
    auto slice = msg->as_rslice();

    REQUIRE_FALSE(fix.responder.start_tx_from_upper(slice));
}

TEST_CASE(SUITE("won't transmit if no session"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();
    auto msg = HexConversions::from_hex("");
    auto slice = msg->as_rslice();

    REQUIRE_FALSE(fix.responder.start_tx_from_upper(slice));
}

TEST_CASE(SUITE("can transmit multiple messages if session is initialized"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();
    test_init_session_success(fix);

    const auto payload = "CA FE";

    for (uint16_t i = 0; i < 3; ++i) {
        auto msg = HexConversions::from_hex(payload);
        auto slice = msg->as_rslice();
        REQUIRE(fix.responder.start_tx_from_upper(slice));

        const auto expected = hex::session_data(i + 1, consts::crypto::default_ttl_pad_ms, payload, hex::repeat(0xFF, 16));
        REQUIRE(fix.lower.pop_tx_message() == expected);
        REQUIRE(fix.upper.num_tx_ready == i);

        // tell the responder that we're done transmitting
        fix.responder.on_lower_tx_ready();
        REQUIRE(fix.upper.num_tx_ready == i + 1);
        REQUIRE(fix.lower.num_tx_messages() == 0);
    }
}

TEST_CASE(SUITE("closes upper layer if nonce exceeds configured maximum"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();
    test_init_session_success(fix, 0); //set max nonce to zero

    const auto payload = "CA FE";

    auto msg = HexConversions::from_hex(payload);
    auto slice = msg->as_rslice();

    REQUIRE(fix.responder.start_tx_from_upper(slice));
    REQUIRE(fix.upper.is_empty());
    REQUIRE_FALSE(fix.upper.get_is_open());
}

TEST_CASE(SUITE("defers transmission if lower layer is not tx_ready"))
{
    ResponderFixture fix;
    fix.responder.on_lower_open();
    test_init_session_success(fix);

    const auto payload = "CA FE";

    fix.lower.set_tx_ready(false);

    auto msg = HexConversions::from_hex(payload);
    auto slice = msg->as_rslice();

    REQUIRE(fix.responder.start_tx_from_upper(slice));
    REQUIRE(fix.lower.num_tx_messages() == 0);
    REQUIRE(fix.upper.num_tx_ready == 0);

    fix.lower.set_tx_ready(true);
    fix.responder.on_lower_tx_ready();

    const auto expected = hex::session_data(1, consts::crypto::default_ttl_pad_ms, payload, hex::repeat(0xFF, 16));
    REQUIRE(fix.lower.pop_tx_message() == expected);
    REQUIRE(fix.upper.num_tx_ready == 0);

    fix.responder.on_lower_tx_ready();
    REQUIRE(fix.upper.num_tx_ready == 1);
    REQUIRE(fix.lower.num_tx_messages() == 0);
}

// ---------- helper method implementations -----------

void test_begin_handshake_success(ResponderFixture& fix, uint16_t max_nonce, uint32_t max_session_time)
{
    const auto request = hex::request_handshake_begin(
        0,
        SessionNonceMode::strict_increment,
        HandshakeEphemeral::x25519,
        HandshakeHash::sha256,
        HandshakeKDF::hkdf_sha256,
        SessionCryptoMode::hmac_sha256_16,
        max_nonce,
        max_session_time,
        HandshakeMode::public_keys,
        hex::repeat(0xFF, consts::crypto::x25519_key_length));

    fix.lower.enqueue_message(request);

    // expected order of crypto operations
    fix.expect(
        { CryptoAction::gen_keypair_x25519, // generate local ephemeral key pair
          CryptoAction::hash_sha256,        // mix ck of received message
          CryptoAction::hash_sha256,        // mix ck of transmitted message
          CryptoAction::dh_x25519,          // 3 DH operations to calculate ak and ck
          CryptoAction::dh_x25519,
          CryptoAction::dh_x25519,
          CryptoAction::hkdf_sha256 });

    const auto reply = hex::reply_handshake_begin(hex::repeat(0xFF, consts::crypto::x25519_key_length));
    REQUIRE(fix.lower.pop_tx_message() == reply);
    fix.set_tx_ready();
}

void test_auth_handshake_success(ResponderFixture& fix, const std::string& payload)
{
    const auto mac_hex = hex::repeat(0xFF, consts::crypto::sha256_hash_output_length);

    fix.lower.enqueue_message(hex::session_data(0, 0xFFFFFFFF, payload, hex::repeat(0xFF, 16)));

    const auto reply = hex::session_data(0, 0x00002710, "", hex::repeat(0xFF, 16));

    REQUIRE(fix.lower.pop_tx_message() == reply);

    // expected order of crypto operations
    fix.expect(
        {
            CryptoAction::hmac_sha256,   // calculate expected value
            CryptoAction::secure_equals, // compare MAC values
            CryptoAction::hmac_sha256,   // calculate reply value
        });

    fix.set_tx_ready();
}

void test_init_session_success(ResponderFixture& fix, uint16_t max_nonce, uint32_t max_session_time)
{
    test_begin_handshake_success(fix, max_nonce, max_session_time);
    test_auth_handshake_success(fix, "");

    REQUIRE(fix.upper.get_is_open());
}

void test_handshake_error(ResponderFixture& fix, const std::string& request, HandshakeError expected_error, std::initializer_list<CryptoAction> actions)
{
    fix.lower.enqueue_message(request);

    fix.expect(actions);

    REQUIRE(fix.lower.pop_tx_message() == hex::reply_handshake_error(expected_error));
    fix.set_tx_ready();
}
