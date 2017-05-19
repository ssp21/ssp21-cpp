

#include "catch.hpp"

#include "ssp21/link/CastagnoliCRC32.h"
#include "testlib/Hex.h"

#define SUITE(name) "CRCTestSuite - " name

using namespace ssp21;
using namespace openpal;

/*

These test vector both agree with:

https://www.ghsi.de/CRC/
http://www.sunshine2k.de/coding/javascript/crc/crc_js.html

**/

TEST_CASE(SUITE("Castagnoli test vector #1"))
{
    Hex hex("31 32 33 34 35 36 37 38 39");
    REQUIRE(CastagnoliCRC32::calc(hex) == 0x6C9F84A8);
}

TEST_CASE(SUITE("Castagnoli test vector #2"))
{
    Hex hex("DE AD BE EF");
    REQUIRE(CastagnoliCRC32::calc(hex) == 0xBB4E2049);
}

