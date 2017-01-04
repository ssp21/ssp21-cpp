
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

void init(Session& session, Timestamp session_init_time = Timestamp(0));
std::string validate(Session& session, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::error_code& ec);
void test_validation_failure(const Session::Config& config, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::initializer_list<CryptoAction> actions, CryptoError error);
std::string test_validation_success(const Session::Config& config, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex);

void test_format_failure(const Session::Config& config, int64_t session_init_time, const std::shared_ptr<IFrameWriter>& frame_writer, const Timestamp& now, const std::string& clear_text, const std::error_code& expected);

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
    REQUIRE(test_user_data == test_validation_success(Session::Config(), Timestamp(0), 1, 0, 0, test_user_data, test_auth_tag));
}

TEST_CASE(SUITE("won't intialize with invalid keys"))
{
    Session session(std::make_shared<MockFrameWriter>());
    REQUIRE_FALSE(session.initialize(Algorithms::Session(), Timestamp(0), SessionKeys()));
}

TEST_CASE(SUITE("empty max results in mac_auth_fail"))
{
    test_validation_failure(Session::Config(), Timestamp(0), 1, 0, 0, "", "", { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::mac_auth_fail);
}

TEST_CASE(SUITE("rejects empty user data"))
{
    test_validation_failure(Session::Config(), Timestamp(0), 1, 0, 0, "", test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::empty_user_data);
}

TEST_CASE(SUITE("rejects data if max session time exceeded"))
{
    Session::Config config;
    config.max_session_time = 10000;

    test_validation_failure(config, Timestamp(0), 1, config.max_session_time, config.max_session_time + 1, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::max_session_time_exceeded);
}

TEST_CASE(SUITE("rejects data if clock rollback detected"))
{
    test_validation_failure(Session::Config(), Timestamp(1), 1, 1, 0, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::clock_rollback);
}

//// ---- validation nonce tests ----

TEST_CASE(SUITE("rejects initial nonce of zero with nonce replay error"))
{
    test_validation_failure(Session::Config(), Timestamp(0), 0, 0, 0, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::nonce_replay);
}

TEST_CASE(SUITE("rejects nonce of 1 when initialized with maximum nonce of zero"))
{
    Session::Config config;
    config.max_nonce = 0;

    test_validation_failure(config, Timestamp(0), 1, 0, 0, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::max_nonce_exceeded);
}

//// ---- validation ttl tests ----

TEST_CASE(SUITE("accepts minimum ttl"))
{
    const auto init_time = Timestamp(4);
    const auto ttl = 3;

    REQUIRE(test_user_data == test_validation_success(Session::Config(), init_time, 1, ttl, init_time.milliseconds + ttl, test_user_data, test_auth_tag));
}

TEST_CASE(SUITE("rejects minimum ttl + 1"))
{
    const auto init_time = Timestamp(4);
    const auto ttl = 3;

    test_validation_failure(Session::Config(), init_time, 1, ttl, init_time.milliseconds + ttl + 1, test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::expired_ttl);
}


//// ---- formatting tests ----

TEST_CASE(SUITE("can't format a message without a valid session"))
{
    Session s(std::make_shared<MockFrameWriter>());
    Hex hex("CAFE");

    auto input = hex.as_rslice();


    std::error_code ec;
    const auto data = s.format_session_message(true, Timestamp(0), input, ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(input.length() == 2);
    REQUIRE(data.is_empty());
}

TEST_CASE(SUITE("can't format a message with maximum nonce value already reached"))
{
    Session::Config config;
    config.max_nonce = 0;

    test_format_failure(config, 0, std::make_shared<MockFrameWriter>(), Timestamp(0), "CA FE", CryptoError::max_nonce_exceeded);
}

TEST_CASE(SUITE("can't format a message if the session time exceeds the configured time"))
{
    Session::Config config;
    config.max_session_time = 60;

    test_format_failure(config, 0, std::make_shared<MockFrameWriter>(), Timestamp(config.max_session_time + 1), "CA FE", CryptoError::max_session_time_exceeded);
}

TEST_CASE(SUITE("won't format a maximum if the clock has rolled back since initialization"))
{
    test_format_failure(Session::Config(), 1, std::make_shared<MockFrameWriter>(), Timestamp(0), "CA FE", CryptoError::clock_rollback);
}

TEST_CASE(SUITE("won't format a maximum if adding the TTL would exceed the maximum session time"))
{
    Session::Config config;
    config.max_session_time = consts::crypto::default_ttl_pad_ms - 1;

    test_format_failure(config, 0, std::make_shared<MockFrameWriter>(), Timestamp(0), "CA FE", CryptoError::max_session_time_exceeded);
}

TEST_CASE(SUITE("forwards the formatting error if the session::write function can't write to the output buffer"))
{
    test_format_failure(Session::Config(), 0, std::make_shared<MockFrameWriter>(openpal::Logger::empty(), 0), Timestamp(0), "CA FE", CryptoError::bad_buffer_size);
}

TEST_CASE(SUITE("successfully formats and increments nonce"))
{
    CryptoTest test;

    Session s(std::make_shared<MockFrameWriter>());
    init(s);
    Hex hex("CAFE");

    std::error_code ec;

    for (uint16_t nonce = 1; nonce < 4; ++nonce)
    {

        auto input = hex.as_rslice();
        std::error_code ec;

        const auto data = s.format_session_message(true, Timestamp(0), input, ec);
        REQUIRE_FALSE(ec);
        REQUIRE(input.is_empty());
        test->expect({ CryptoAction::hmac_sha256 });
    }

}

/// ------- helpers methods impls -------------

void init(Session& session, Timestamp session_init_time)
{
    SessionKeys keys;
    keys.rx_key.set_type(BufferType::symmetric_key);
    keys.tx_key.set_type(BufferType::symmetric_key);

    REQUIRE(session.initialize(Algorithms::Session(), session_init_time, keys));
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

std::string test_validation_success(const Session::Config& config, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex)
{
    CryptoTest crypto;

    Session session(std::make_shared<MockFrameWriter>(), config);

    init(session, session_init_time);

    std::error_code ec;
    const auto user_data = validate(session, nonce, ttl, now, user_data_hex, auth_tag_hex, ec);
    REQUIRE_FALSE(ec);

    crypto->expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });

    return user_data;
}

void test_validation_failure(const Session::Config& config, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::initializer_list<CryptoAction> actions, CryptoError error)
{
    CryptoTest crypto;

    Session session(std::make_shared<MockFrameWriter>(), config);

    init(session, session_init_time);

    std::error_code ec;
    const auto user_data = validate(session, nonce, ttl, now, user_data_hex, auth_tag_hex, ec);
    REQUIRE(ec == error);
    REQUIRE(user_data.empty());

    crypto->expect(actions);

}

void test_format_failure(const Session::Config& config, int64_t session_init_time, const std::shared_ptr<IFrameWriter>& frame_writer, const Timestamp& now, const std::string& clear_text, const std::error_code& expected)
{
    Session s(frame_writer, config);
    init(s, Timestamp(session_init_time));

    Hex hex(clear_text);
    auto input = hex.as_rslice();
    const auto start_length = input.length();

    std::error_code ec;
    const auto data = s.format_session_message(true, now, input, ec);
    REQUIRE(ec == expected);
    REQUIRE(input.length() == start_length);
    REQUIRE(data.is_empty());
}