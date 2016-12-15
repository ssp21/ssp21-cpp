
#include "catch.hpp"

#include "ssp21/crypto/Session.h"
#include "ssp21/crypto/gen/CryptoError.h"

#define SUITE(name) "SessionTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("can't validate user data when not initialized"))
{
    Session session(10);

    UnconfirmedSessionData msg(
        AuthMetadata(
            0,
            0xFFFFFFFF,
            SessionFlags(true, true)
        ),
        Seq16()
    );


    std::error_code ec;
    const auto user_data = session.validate_user_data(msg, Timestamp(0), ec);
    REQUIRE(ec == CryptoError::no_valid_session);
    REQUIRE(user_data.is_empty());
}



