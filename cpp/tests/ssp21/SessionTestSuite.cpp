
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

void init(Session& session, verify_nonce_func_t verify_nonce = NonceFunctions::default_verify(), uint16_t nonce_start = 0);
RSlice validate(Session& session, uint16_t nonce, uint32_t ttl, uint32_t now, const std::string& payload, std::error_code& ec);

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

TEST_CASE(SUITE("won't intialize with invalid keys"))
{
	CryptoTest crypto;

    Session session(10);
    REQUIRE_FALSE(session.initialize(Algorithms::Session(), Timestamp(0), SessionKeys()));
	
}

TEST_CASE(SUITE("returns size errors from the session read function"))
{
	CryptoTest crypto;

	Session session(10);
	init(session);

	std::error_code ec;
	const auto user_data = validate(session, 1, 0, 0, "", ec);
	REQUIRE(ec == CryptoError::bad_buffer_size);
	REQUIRE(user_data.is_empty());
	
	crypto->expect_empty();
}

TEST_CASE(SUITE("authenticates data"))
{
	CryptoTest crypto;

    Session session(10);
    init(session);

    std::error_code ec;
    const auto user_data = validate(session, 1, 0, 0, "CAFE" + repeat_hex(0xFF, consts::crypto::trunc16), ec);
	REQUIRE_FALSE(ec);
	REQUIRE("CA FE" == to_hex(user_data));

	crypto->expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });
}

//// ---- nonce tests ----

TEST_CASE(SUITE("rejects initial nonce of zero"))
{
	CryptoTest crypto;

	Session session(10);
	init(session);

	std::error_code ec;
	const auto user_data = validate(session, 0, 0, 0, "CAFE" + repeat_hex(0xFF, consts::crypto::trunc16), ec);
	REQUIRE(ec == CryptoError::invalid_nonce);
	REQUIRE(user_data.is_empty());

	crypto->expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });
}

TEST_CASE(SUITE("rejects rollover nonce when initialized with maximum nonce"))
{
	CryptoTest crypto;

	Session session(10);
	init(session, NonceFunctions::default_verify(), 65535);

	std::error_code ec;
	const auto user_data = validate(session, 0, 0, 0, "CAFE" + repeat_hex(0xFF, consts::crypto::trunc16), ec);
	REQUIRE(ec == CryptoError::invalid_nonce);
	REQUIRE(user_data.is_empty());

	crypto->expect({ CryptoAction::hmac_sha256, CryptoAction::secure_equals });
}

//// ---- ttl tests ----

/// ------- helpers methods impls -------------

void init(Session& session, verify_nonce_func_t verify_nonce, uint16_t nonce_start)
{
    Algorithms::Session algorithms;
    algorithms.verify_nonce = verify_nonce;

    SessionKeys keys;
    keys.rx_key.set_type(BufferType::symmetric_key);
    keys.tx_key.set_type(BufferType::symmetric_key);

    REQUIRE(session.initialize(algorithms, Timestamp(0), keys, nonce_start));
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