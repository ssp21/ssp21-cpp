
#include "catch.hpp"

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include "mocks/MockCryptoBackend.h"

#include <array>

#define SUITE(name) "SessionTestSuite - " name

using namespace ssp21;
using namespace openpal;

void init(Session& session, uint16_t nonce_init = 0);
RSlice validate(Session& session, uint16_t nonce, uint32_t ttl, uint32_t now, const std::string& payload, std::error_code& ec);
void test_validation_failure(uint16_t nonce_init, uint16_t nonce, uint32_t ttl, uint32_t now, const std::string& payload, std::initializer_list<CryptoAction> actions, CryptoError error);

const auto test_userdata = "CA FE";
const auto test_payload = test_userdata + repeat_hex(0xFF, consts::crypto::trunc16);

TEST_CASE(SUITE("won't validate user data when not initialized"))
{
    CryptoTest crypto;

    Session session(10);
    std::error_code ec;
    const auto user_data = validate(session, 1, 0, 0, "", ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(user_data.is_empty());

    crypto->expect_empty();
}

TEST_CASE(SUITE("authenticates data"))
{
    CryptoTest crypto;

    Session session(10);
    init(session);

    std::error_code ec;
    const auto user_data = validate(session, 1, 0, 0, test_payload, ec);
    REQUIRE_FALSE(ec);
    REQUIRE(test_userdata == to_hex(user_data));

    crypto->expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });
}

TEST_CASE(SUITE("won't intialize with invalid keys"))
{
    Session session(10);
    REQUIRE_FALSE(session.initialize(Algorithms::Session(), Timestamp(0), SessionKeys()));
}

TEST_CASE(SUITE("returns size errors from the session read function"))
{
    test_validation_failure(0, 1, 0, 0, "", {}, CryptoError::bad_buffer_size);
}

//// ---- nonce tests ----

TEST_CASE(SUITE("rejects initial nonce of zero"))
{
    test_validation_failure(0, 0, 0, 0, test_payload, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::invalid_nonce);
}

TEST_CASE(SUITE("rejects rollover nonce when initialized with maximum nonce"))
{
    test_validation_failure(65535, 0, 0, 0, test_payload, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::invalid_nonce);
}

//// ---- ttl tests ----

TEST_CASE(SUITE("rejects expired ttl"))
{
    test_validation_failure(0, 1, 0, 1, test_payload, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::expired_ttl);
}

/// ------- helpers methods impls -------------

void init(Session& session, uint16_t nonce_start)
{
    SessionKeys keys;
    keys.rx_key.set_type(BufferType::symmetric_key);
    keys.tx_key.set_type(BufferType::symmetric_key);

    REQUIRE(session.initialize(Algorithms::Session(), Timestamp(0), keys, nonce_start));
}

RSlice validate(Session& session, uint16_t nonce, uint32_t ttl, uint32_t now, const std::string& payload, std::error_code& ec)
{
    Hex hex(payload);

    UnconfirmedSessionData msg(
        AuthMetadata(
            nonce,
            ttl,
            SessionFlags(true, true)
        ),
        Seq16(hex.as_rslice())
    );

    return session.validate_user_data(msg, Timestamp(now), ec);
}

void test_validation_failure(uint16_t nonce_init, uint16_t nonce, uint32_t ttl, uint32_t now, const std::string& payload, std::initializer_list<CryptoAction> actions, CryptoError error)
{
    CryptoTest crypto;

    Session session(10);

    init(session, nonce_init);

    std::error_code ec;
    const auto user_data = validate(session, nonce, ttl, now, payload, ec);
    REQUIRE(ec == error);
    REQUIRE(user_data.is_empty());

    crypto->expect(actions);

}