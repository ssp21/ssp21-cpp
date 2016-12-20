
#include "catch.hpp"

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/gen/CryptoError.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include "mocks/MockCryptoBackend.h"

#include "mocks/HexMessageBuilders.h"

#include <array>

#define SUITE(name) "SessionTestSuite - " name

using namespace ssp21;
using namespace openpal;

void init(Session& session, uint16_t nonce_init = 0, Timestamp session_init_time = Timestamp(0));
RSlice validate(Session& session, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& payload, std::error_code& ec);
void test_validation_failure(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& payload, std::initializer_list<CryptoAction> actions, CryptoError error);
std::string test_validation_success(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& payload);

const auto test_userdata = "CA FE";
const auto test_payload = test_userdata + repeat_hex(0xFF, consts::crypto::trunc16);

TEST_CASE(SUITE("won't validate user data when not initialized"))
{
    CryptoTest crypto;

    Session session;
    std::error_code ec;
    const auto user_data = validate(session, 1, 0, 0, "", ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(user_data.is_empty());

    crypto->expect_empty();
}

TEST_CASE(SUITE("authenticates data"))
{
    REQUIRE(test_userdata == test_validation_success(0, Timestamp(0), 1, 0, 0, test_payload));
}

TEST_CASE(SUITE("won't intialize with invalid keys"))
{
    Session session;
    REQUIRE_FALSE(session.initialize(Algorithms::Session(), Timestamp(0), SessionKeys()));
}

TEST_CASE(SUITE("returns size errors from the session read function"))
{
    test_validation_failure(0, Timestamp(0), 1, 0, 0, "", {}, CryptoError::bad_buffer_size);
}

//// ---- validation nonce tests ----

TEST_CASE(SUITE("rejects initial nonce of zero"))
{
    test_validation_failure(0, Timestamp(0), 0, 0, 0, test_payload, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::invalid_rx_nonce);
}

TEST_CASE(SUITE("rejects rollover nonce when initialized with maximum nonce"))
{
    test_validation_failure(65535, Timestamp(0), 0, 0, 0, test_payload, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::invalid_rx_nonce);
}

//// ---- validation ttl tests ----

TEST_CASE(SUITE("accepts minimum ttl"))
{
    const auto init_time = Timestamp(4);
    const auto ttl = 3;

    REQUIRE(test_userdata == test_validation_success(0, init_time, 1, ttl, init_time.milliseconds + ttl, test_payload));
}

TEST_CASE(SUITE("rejects minimum ttl + 1"))
{
    const auto init_time = Timestamp(4);
    const auto ttl = 3;

    test_validation_failure(0, init_time, 1, ttl, init_time.milliseconds + ttl + 1, test_payload, { CryptoAction::hmac_sha256, CryptoAction::secure_equals }, CryptoError::expired_ttl);
}


//// ---- formatting tests ----

TEST_CASE(SUITE("can't format a message without a valid session"))
{
    Session s;
    StaticBuffer<consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    std::error_code ec;
    auto input = hex.as_rslice();
    const auto output = s.format_message(buffer.as_wslice(), true, Timestamp(0), input, ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(output.is_empty());
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("can't format a with maximum nonce value"))
{
    Session s;
    init(s, std::numeric_limits<uint16_t>::max());
    StaticBuffer<consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    std::error_code ec;
    auto input = hex.as_rslice();
    const auto output = s.format_message(buffer.as_wslice(), true, Timestamp(0), input, ec);
    REQUIRE(ec == CryptoError::invalid_tx_nonce);
    REQUIRE(output.is_empty());
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("can't format a message if the session time has exceed 2^32 - 1"))
{
    Session s;
    init(s);
    StaticBuffer<consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    std::error_code ec;
    auto input = hex.as_rslice();
    const auto time = static_cast<int64_t>(std::numeric_limits<uint32_t>::max()) + 1;
    const auto output = s.format_message(buffer.as_wslice(), true, Timestamp(time), input, ec);
    REQUIRE(ec == CryptoError::ttl_overflow);
    REQUIRE(output.is_empty());
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("can't format a maximum if the session time has overflowed"))
{
    Session s;
    init(s);
    StaticBuffer<consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    std::error_code ec;
    auto input = hex.as_rslice();
    const auto time = std::numeric_limits<uint32_t>::max() - consts::crypto::default_ttl_pad_ms + 1;
    const auto output = s.format_message(buffer.as_wslice(), true, Timestamp(time), input, ec);
    REQUIRE(ec == CryptoError::ttl_overflow);
    REQUIRE(output.is_empty());
    REQUIRE(input.length() == 2);
}

TEST_CASE(SUITE("can't format if destination buffer is too small"))
{
    Session s;
    init(s);
    StaticBuffer<4> buffer;
    Hex hex("CAFE");

    std::error_code ec;
    auto input = hex.as_rslice();
    const auto output = s.format_message(buffer.as_wslice(), true, Timestamp(0), input, ec);
    REQUIRE(ec == FormatError::insufficient_space);
    REQUIRE(output.is_empty());
}

TEST_CASE(SUITE("successfully formats and increments nonce"))
{
    CryptoTest test;

    Session s;
    init(s);
    StaticBuffer<consts::link::max_config_payload_size> buffer;
    Hex hex("CAFE");

    std::error_code ec;

    for (uint16_t nonce = 1; nonce < 4; ++nonce)
    {

        auto input = hex.as_rslice();
        const auto output = s.format_message(buffer.as_wslice(), true, Timestamp(0), input, ec);
        const auto expected = hex::session_data(nonce, 10000, true, true, "CA FE" + hex::repeat(0xFF, consts::crypto::trunc16));
        REQUIRE_FALSE(ec);
        REQUIRE(to_hex(output) == expected);
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

RSlice validate(Session& session, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& payload, std::error_code& ec)
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

    return session.validate_message(msg, Timestamp(now), ec);
}

std::string test_validation_success(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& payload)
{
    CryptoTest crypto;

    Session session;

    init(session, nonce_init, session_init_time);

    std::error_code ec;
    const auto user_data = validate(session, nonce, ttl, now, payload, ec);
    REQUIRE_FALSE(ec);

    crypto->expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });

    return to_hex(user_data);
}

void test_validation_failure(uint16_t nonce_init, Timestamp session_init_time, uint16_t nonce, uint32_t ttl, int64_t now, const std::string& payload, std::initializer_list<CryptoAction> actions, CryptoError error)
{
    CryptoTest crypto;

    Session session;

    init(session, nonce_init, session_init_time);

    std::error_code ec;
    const auto user_data = validate(session, nonce, ttl, now, payload, ec);
    REQUIRE(ec == error);
    REQUIRE(user_data.is_empty());

    crypto->expect(actions);

}