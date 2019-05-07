

#include "catch.hpp"

#include "ssp21/link/CastagnoliCRC32.h"
#include "ser4cpp/util/HexConversions.h"

#define SUITE(name) "CRCTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

/*

These test vectors agree with:

http://www.sunshine2k.de/coding/javascript/crc/crc_js.html

The proper settings for the online generator are:
CRC Width = CRC-32
Input Reflected = No
Result Reflected = No
Polynomial = 0xf4acfb13
Initial Value = 0x0000000
Final XOR Value = 0x0000000

*/

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

TEST_CASE(SUITE("Castagnoli test vector #3"))
{
	const std::string data("abcdefghijklmnopqrstuvwxyz012345");
	const auto slice = rseq_t(reinterpret_cast<const uint8_t*>(data.c_str()), static_cast<uint32_t>(data.length()));
	REQUIRE(CastagnoliCRC32::calc(slice) == 0x242D5EBD);
}

