

#include "catch.hpp"

#include "ssp21/crypto/Crypto.h"

#define SUITE(name) "SecureMemoryTestSuite - " name

using namespace ssp21;

// not testing for constant time here, just that the
// binding to sodium is correct...
TEST_CASE(SUITE("test secure comparison"))
{
    uint8_t buff1[3] = { 0x01, 0x02, 0x03 };
    uint8_t buff2[3] = { 0x01, 0x02, 0x04 };
    uint8_t buff3[4] = { 0x01, 0x02, 0x03, 0x04 };

    auto slice1 = seq32_t(buff1, 3);
    auto slice2 = seq32_t(buff2, 3);
    auto slice3 = seq32_t(buff3, 4);

    REQUIRE(Crypto::secure_equals(slice1, slice1));
    REQUIRE_FALSE(Crypto::secure_equals(slice1, slice2));
    REQUIRE_FALSE(Crypto::secure_equals(slice1, slice3));
}
