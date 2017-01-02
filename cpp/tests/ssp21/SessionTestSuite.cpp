
#include "catch.hpp"

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include "testlib/HexConversions.h"

#include "mocks/MockCryptoBackend.h"

#include "mocks/HexMessageBuilders.h"
#include "mocks/HexSequences.h"
#include "mocks/MockFrameWriter.h"

#include <array>

#define SUITE(name) "SessionTestSuite - " name

using namespace ssp21;
using namespace openpal;

void init(Session& session, uint16_t nonce_init = 0, Timestamp session_init_time = Timestamp(0));
std::string validate(Session& session, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::error_code& ec);
void test_validation_failure(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::initializer_list<CryptoAction> actions, CryptoError error);
std::string test_validation_success(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex);

const auto test_user_data = "CA FE";
const auto test_auth_tag = repeat_hex(0xFF, consts::crypto::trunc16);

TEST_CASE(SUITE("won't validate user data when not initialized"))
{
    CryptoTest crypto;

	Session session(std::make_shared<MockFrameWriter>());
    std::error_code ec;
    const auto user_data = validate(session, 1, 0, 0, "", "", ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(user_data.empty());

    crypto->expect_empty();
}

TEST_CASE(SUITE("authenticates data"))
{
    REQUIRE(test_user_data == test_validation_success(0, Timestamp(0), 1, 0, 0, test_user_data, test_auth_tag));
}

TEST_CASE(SUITE("won't intialize with invalid keys"))
{
    Session session(std::make_shared<MockFrameWriter>());
    REQUIRE_FALSE(session.initialize(Algorithms::Session(), Timestamp(0), SessionKeys()));
}

TEST_CASE(SUITE("empty max results in mac_auth_fail"))
{
    test_validation_failure(0, Timestamp(0), 1, 0, 0, "", "", { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::mac_auth_fail);
}

TEST_CASE(SUITE("rejects empty user data"))
{
    test_validation_failure(0, Timestamp(0), 1, 0, 0, "", test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::empty_user_data);
}

//// ---- validation nonce tests ----

TEST_CASE(SUITE("rejects initial nonce of zero"))
{
    test_validation_failure(0, Timestamp(0), 0, 0, 0, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::invalid_rx_nonce);
}

TEST_CASE(SUITE("rejects rollover nonce when initialized with maximum nonce"))
{
    test_validation_failure(65535, Timestamp(0), 0, 0, 0, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::invalid_rx_nonce);
}

//// ---- validation ttl tests ----

TEST_CASE(SUITE("accepts minimum ttl"))
{
    const auto init_time = Timestamp(4);
    const auto ttl = 3;

    REQUIRE(test_user_data == test_validation_success(0, init_time, 1, ttl, init_time.milliseconds + ttl, test_user_data, test_auth_tag));
}

TEST_CASE(SUITE("rejects minimum ttl + 1"))
{
    const auto init_time = Timestamp(4);
    const auto ttl = 3;

    test_validation_failure(0, init_time, 1, ttl, init_time.milliseconds + ttl + 1, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::expired_ttl);
}


//// ---- formatting tests ----

TEST_CASE(SUITE("can't format a message without a valid session"))
{
    Session s(std::make_shared<MockFrameWriter>());
    StaticBuffer<uint32_t, consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    auto input = hex.as_rslice();
    SessionData msg;
    const auto err = s.format_message(msg, true, Timestamp(0), input);
    REQUIRE(err == CryptoError::no_valid_session);
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("can't format a with maximum nonce value"))
{
    Session s(std::make_shared<MockFrameWriter>());
    init(s, std::numeric_limits<uint16_t>::max());
    StaticBuffer<uint32_t, consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");


    auto input = hex.as_rslice();
    SessionData msg;
    const auto err = s.format_message(msg, true, Timestamp(0), input);
    REQUIRE(err == CryptoError::invalid_tx_nonce);
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("can't format a message if the session time has exceed 2^32 - 1"))
{
    Session s(std::make_shared<MockFrameWriter>());
    init(s);
    StaticBuffer<uint32_t, consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    auto input = hex.as_rslice();
    SessionData msg;
    const auto time = static_cast<int64_t>(std::numeric_limits<uint32_t>::max()) + 1;
    const auto err = s.format_message(msg, true, Timestamp(time), input);
    REQUIRE(err == CryptoError::ttl_overflow);
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("can't format a maximum if the session time has overflowed"))
{
    Session s(std::make_shared<MockFrameWriter>());
    init(s);
    StaticBuffer<uint32_t, consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    auto input = hex.as_rslice();
    SessionData msg;

    const auto time = std::numeric_limits<uint32_t>::max() - consts::crypto::default_ttl_pad_ms + 1;
    const auto err = s.format_message(msg, true, Timestamp(time), input);
    REQUIRE(err == CryptoError::ttl_overflow);
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("successfully formats and increments nonce"))
{
    CryptoTest test;

    Session s(std::make_shared<MockFrameWriter>());
    init(s);
    StaticBuffer<uint32_t, consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    std::error_code ec;

    for (uint16_t nonce = 1; nonce < 4; ++nonce)
    {

        auto input = hex.as_rslice();
        SessionData msg;

        const auto err = s.format_message(msg, true, Timestamp(0), input);
        REQUIRE_FALSE(err);
        REQUIRE(input.is_empty());
        test->expect({ CryptoAction::hmac_sha256 });
    }

}

/// ------- helpers methods impls -------------

void init(Session& session, uint16_t nonce_start, Timestamp session_init_time)
{
    SessionKeys keys;
    keys.rx_key.set_type(BufferType::symmetric_key);
    keys.tx_key.set_type(BufferType::symmetric_key);

    REQUIRE(session.initialize(Algorithms::Session(), session_init_time, keys, nonce_start));
}

std::string validate(Session& session, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::error_code& ec)
{
    HexSeq16 user_data(user_data_hex);
    HexSeq8 auth_tag(auth_tag_hex);

    SessionData msg(
        AuthMetadata(
            nonce,
            ttl,
            SessionFlags(true, true)
        ),
        user_data,
        auth_tag
    );

    return to_hex(session.validate_message(msg, Timestamp(now), ec));
}

std::string test_validation_success(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex)
{
    CryptoTest crypto;

    Session session(std::make_shared<MockFrameWriter>());

    init(session, nonce_init, session_init_time);

    std::error_code ec;
    const auto user_data = validate(session, nonce, ttl, now, user_data_hex, auth_tag_hex, ec);
    REQUIRE_FALSE(ec);

    crypto->expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });

    return user_data;
}

void test_validation_failure(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::initializer_list<CryptoAction> actions, CryptoError error)
{
    CryptoTest crypto;

    Session session(std::make_shared<MockFrameWriter>());

    init(session, nonce_init, session_init_time);

    std::error_code ec;
    const auto user_data = validate(session, nonce, ttl, now, user_data_hex, auth_tag_hex, ec);
    REQUIRE(ec == error);
    REQUIRE(user_data.empty());

    crypto->expect(actions);

}