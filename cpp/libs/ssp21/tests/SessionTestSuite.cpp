
#include "catch.hpp"

#include "crypto/MessageOnlyFrameWriter.h"
#include "crypto/Session.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include "mocks/CryptoFixture.h"

#include "mocks/HexMessageBuilders.h"
#include "mocks/HexSequences.h"

#include "ser4cpp/util/HexConversions.h"

#include <array>

#define SUITE(name) "SessionTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

const auto test_user_data = "CA FE";
const auto test_auth_tag = HexConversions::repeat_hex(0xFF, consts::crypto::trunc16);

struct SessionFixture {
    const std::shared_ptr<MessageOnlyFrameWriter> frame_writer;
    const std::shared_ptr<SessionStatistics> statistics;
    CryptoFixture crypto;
    Session session;

    SessionFixture(const SessionConfig& config = SessionConfig())
        : frame_writer(std::make_shared<MessageOnlyFrameWriter>())
        , statistics(std::make_shared<SessionStatistics>())
        , session(frame_writer, statistics, config)
    {
    }

    SessionFixture(const std::shared_ptr<MessageOnlyFrameWriter>& frame_writer)
        : frame_writer(frame_writer)
        , statistics(std::make_shared<SessionStatistics>())
        , session(frame_writer, statistics, SessionConfig())
    {
    }

    void init(const Session::Param& parameters = Session::Param());
    std::string validate(uint16_t nonce, uint32_t ttl, const exe4cpp::steady_time_t& now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::error_code& ec);
    void test_validation_failure(const Session::Param& parameters, uint16_t nonce, uint32_t ttl, const exe4cpp::steady_time_t& now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::initializer_list<CryptoAction> actions, CryptoError error);
    std::string test_validation_success(const Session::Param& parameters, uint16_t nonce, uint32_t ttl, const exe4cpp::steady_time_t& now, const std::string& user_data_hex, const std::string& auth_tag_hex);
    void test_format_failure(const Session::Param& parameters, const exe4cpp::steady_time_t& now, const std::string& clear_text, const std::error_code& expected);
};

TEST_CASE(SUITE("won't validate user data when not initialized"))
{
    SessionFixture fixture;

    std::error_code ec;
    const auto user_data = fixture.validate(1, 0, exe4cpp::steady_time_t(), "", "", ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(user_data.empty());

    fixture.crypto.expect_empty();
}

TEST_CASE(SUITE("authenticates data"))
{
    SessionFixture fixture;
    REQUIRE(test_user_data == fixture.test_validation_success(Session::Param(), 1, 0, exe4cpp::steady_time_t(), test_user_data, test_auth_tag));
}

TEST_CASE(SUITE("won't intialize with invalid keys"))
{
    SessionFixture fixture;
    REQUIRE_FALSE(fixture.session.initialize(Algorithms::Session(), Session::Param(), SessionKeys()));
}

TEST_CASE(SUITE("empty max results in mac_auth_fail"))
{
    SessionFixture fixture;
    fixture.test_validation_failure(Session::Param(), 1, 0, exe4cpp::steady_time_t(), "", "", { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::mac_auth_fail);
}

TEST_CASE(SUITE("rejects empty user data"))
{
    SessionFixture fixture;
    fixture.test_validation_failure(Session::Param(), 1, 0, exe4cpp::steady_time_t(), "", test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::empty_user_data);
}

TEST_CASE(SUITE("rejects data if max session time exceeded"))
{
    Session::Param parameters;

    SessionFixture fixture;
    fixture.test_validation_failure(parameters, 1,
                                    static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(parameters.max_session_time).count()),
                                    exe4cpp::steady_time_t() + parameters.max_session_time + std::chrono::milliseconds(1),
                                    test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::max_session_time_exceeded);
}

TEST_CASE(SUITE("rejects data if clock rollback detected"))
{
    Session::Param param;
    param.session_start = exe4cpp::steady_time_t(std::chrono::seconds(10));
    SessionFixture fixture;
    fixture.test_validation_failure(param, 1, 1, exe4cpp::steady_time_t(), test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::clock_rollback);
}

//// ---- validation nonce tests ----

TEST_CASE(SUITE("rejects initial nonce of zero with nonce replay error"))
{
    SessionFixture fixture;
    fixture.test_validation_failure(Session::Param(), 0, 0, exe4cpp::steady_time_t(), test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::nonce_replay);
}

TEST_CASE(SUITE("rejects nonce of 1 when initialized with maximum nonce of zero"))
{
    Session::Param param;
    param.max_nonce = 0;

    SessionFixture fixture;
    fixture.test_validation_failure(param, 1, 0, exe4cpp::steady_time_t(), test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::max_nonce_exceeded);
}

//// ---- validation ttl tests ----

TEST_CASE(SUITE("accepts minimum ttl"))
{
    Session::Param param;
    param.session_start = exe4cpp::steady_time_t();
    const auto ttl = 3;

    SessionFixture fixture;
    REQUIRE(test_user_data == fixture.test_validation_success(param, 1, ttl, param.session_start + std::chrono::milliseconds(ttl), test_user_data, test_auth_tag));
}

TEST_CASE(SUITE("rejects minimum ttl + 1"))
{
    Session::Param param;
    param.session_start = exe4cpp::steady_time_t();
    const auto ttl = 3;

    SessionFixture fixture;
    fixture.test_validation_failure(param, 1, ttl, param.session_start + std::chrono::milliseconds(ttl + 1), test_user_data, test_auth_tag, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::expired_ttl);
}

//// ---- formatting tests ----

TEST_CASE(SUITE("can't format a message without a valid session"))
{
    SessionFixture fixture;

    auto input = HexConversions::from_hex("CAFE")->as_rslice();

    std::error_code ec;
    const auto data = fixture.session.format_session_data(exe4cpp::steady_time_t(), input, ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(input.length() == 2);
    REQUIRE(data.is_empty());
}

TEST_CASE(SUITE("can't format a message with maximum nonce value already reached"))
{
    Session::Param param;
    param.max_nonce = 0;

    SessionFixture fixture;
    fixture.test_format_failure(param, exe4cpp::steady_time_t(), "CA FE", CryptoError::max_nonce_exceeded);
}

TEST_CASE(SUITE("can't format a message if the session time exceeds the configured time"))
{
    Session::Param param;
    param.max_session_time = std::chrono::milliseconds(60);

    SessionFixture fixture;
    fixture.test_format_failure(param, exe4cpp::steady_time_t() + param.max_session_time + std::chrono::milliseconds(1), "CA FE", CryptoError::max_session_time_exceeded);
}

TEST_CASE(SUITE("won't format a message if the clock has rolled back since initialization"))
{
    Session::Param param;
    param.session_start = exe4cpp::steady_time_t();

    SessionFixture fixture;
    fixture.test_format_failure(param, param.session_start - std::chrono::milliseconds(1), "CA FE", CryptoError::clock_rollback);
}

TEST_CASE(SUITE("won't format a message if adding the TTL would exceed the maximum session time"))
{
    Session::Param param;
    param.max_session_time = std::chrono::milliseconds(consts::crypto::default_ttl_pad_ms - 1);

    SessionFixture fixture;
    fixture.test_format_failure(param, exe4cpp::steady_time_t(), "CA FE", CryptoError::max_session_time_exceeded);
}

TEST_CASE(SUITE("forwards the formatting error if the session::write function can't write to the output buffer"))
{
    SessionFixture fixture(
        std::make_shared<MessageOnlyFrameWriter>(log4cpp::Logger::empty(), 0));

    fixture.test_format_failure(Session::Param(), exe4cpp::steady_time_t(), "CA FE", CryptoError::bad_buffer_size);
}

TEST_CASE(SUITE("successfully formats and increments nonce"))
{
    SessionFixture fixture;
    fixture.init();

    auto data = HexConversions::from_hex("CAFE");

    for (uint16_t nonce = 1; nonce < 4; ++nonce) {
        std::error_code ec;
        auto input = data->as_rslice();

        const auto output = fixture.session.format_session_data(exe4cpp::steady_time_t(), input, ec);
        REQUIRE_FALSE(ec);
        REQUIRE(input.is_empty());
        REQUIRE(output.is_not_empty());
        fixture.crypto.expect({ CryptoAction::hmac_sha256 });
    }
}

// ------- helpers methods impls -------------

void SessionFixture::init(const Session::Param& parameters)
{
    SessionKeys keys;
    keys.rx_key.set_type(BufferType::symmetric_key);
    keys.tx_key.set_type(BufferType::symmetric_key);

    REQUIRE(this->session.initialize(Algorithms::Session(), parameters, keys));
}

std::string SessionFixture::validate(uint16_t nonce, uint32_t ttl, const exe4cpp::steady_time_t& now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::error_code& ec)
{
    const HexSeq user_data(user_data_hex);
    const HexSeq auth_tag(auth_tag_hex);

    const SessionData msg(
        AuthMetadata(
            nonce,
            ttl),
        user_data,
        auth_tag);

    ser4cpp::StaticBuffer<uint32_t, 1024> buffer;
    const auto result = this->session.validate_session_data(msg, now, buffer.as_wseq(), ec);

    return HexConversions::to_hex(result);
}

std::string SessionFixture::test_validation_success(const Session::Param& parameters, uint16_t nonce, uint32_t ttl, const exe4cpp::steady_time_t& now, const std::string& user_data_hex, const std::string& auth_tag_hex)
{
    this->init(parameters);

    std::error_code ec;
    const auto user_data = this->validate(nonce, ttl, now, user_data_hex, auth_tag_hex, ec);
    REQUIRE_FALSE(ec);

    crypto.expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });

    return user_data;
}

void SessionFixture::test_validation_failure(const Session::Param& parameters, uint16_t nonce, uint32_t ttl, const exe4cpp::steady_time_t& now, const std::string& user_data_hex, const std::string& auth_tag_hex, std::initializer_list<CryptoAction> actions, CryptoError error)
{
    this->init(parameters);

    std::error_code ec;
    const auto user_data = this->validate(nonce, ttl, now, user_data_hex, auth_tag_hex, ec);
    REQUIRE(ec == error);
    REQUIRE(user_data.empty());

    crypto.expect(actions);
}

void SessionFixture::test_format_failure(const Session::Param& parameters, const exe4cpp::steady_time_t& now, const std::string& clear_text, const std::error_code& expected)
{
    this->init(parameters);

    auto input = HexConversions::from_hex(clear_text)->as_rslice();
    const auto start_length = input.length();

    std::error_code ec;
    const auto data = this->session.format_session_data(now, input, ec);
    REQUIRE(ec == expected);
    REQUIRE(input.length() == start_length);
    REQUIRE(data.is_empty());
}
