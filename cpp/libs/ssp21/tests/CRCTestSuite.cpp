

#include "catch.hpp"

#include "link/CastagnoliCRC32.h"
#include "ser4cpp/util/HexConversions.h"

#define SUITE(name) "CRCTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

/*

These test vector both agree with:

https://www.ghsi.de/CRC/
http://www.sunshine2k.de/coding/javascript/crc/crc_js.html

**/

TEST_CASE(SUITE("Castagnoli test vector #1"))
{
    const auto hex = HexConversions::from_hex("31 32 33 34 35 36 37 38 39");
    REQUIRE(CastagnoliCRC32::calc(hex->as_rslice()) == 0x6C9F84A8);
}

TEST_CASE(SUITE("Castagnoli test vector #2"))
{
    const auto hex = HexConversions::from_hex("DE AD BE EF");
    REQUIRE(CastagnoliCRC32::calc(hex->as_rslice()) == 0xBB4E2049);
}

