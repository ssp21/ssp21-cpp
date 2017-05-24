

#include "catch.hpp"

#include "ssp21/crypto/VLength.h"

#include "openpal/container/StaticBuffer.h"
#include "testlib/HexConversions.h"

#define SUITE(name) "VLenthTestSuite - " name

using namespace ssp21;
using namespace openpal;

void test_encoding(uint32_t number, const std::string& hex)
{
    StaticBuffer<uint32_t, 5> buffer;
    auto dest = buffer.as_wseq();
    REQUIRE(VLength::write(number, dest) == FormatError::ok);
    const auto length = buffer.size() - dest.length();
    REQUIRE(to_hex(buffer.as_seq(length)) == hex);
}

TEST_CASE(SUITE("encodes 127"))
{
    test_encoding(127, "7F");
}

TEST_CASE(SUITE("encodes 128"))
{
    test_encoding(128, "80 01");
}