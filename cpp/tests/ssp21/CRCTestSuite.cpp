

#include "catch.hpp"

#include "ssp21/link/KoopmanCRC32.h"
#include "ssp21/link/CastagnoliCRC32.h"
#include "testlib/BufferHelpers.h"

#define SUITE(name) "CRCTestSuite - " name

using namespace ssp21;
using namespace openpal;

/*

These test vector both agree with:

https://www.ghsi.de/CRC/
http://www.sunshine2k.de/coding/javascript/crc/crc_js.html

**/

TEST_CASE(SUITE("Koopman test vector #1"))
{		
	HexSequence hex("31 32 33 34 35 36 37 38 39");
	REQUIRE(KoopmanCRC32::calc(hex.as_rslice()) == 0x9601DDA9);
}

TEST_CASE(SUITE("Koopman test vector #2"))
{
	HexSequence hex("DE AD BE EF");
	REQUIRE(KoopmanCRC32::calc(hex.as_rslice()) == 0xFDBCA758);
}

TEST_CASE(SUITE("Castagnoli test vector #1"))
{
	HexSequence hex("31 32 33 34 35 36 37 38 39");
	REQUIRE(CastagnoliCRC32::calc(hex.as_rslice()) == 0x6C9F84A8);
}

TEST_CASE(SUITE("Castagnoli test vector #2"))
{
	HexSequence hex("DE AD BE EF");
	REQUIRE(CastagnoliCRC32::calc(hex.as_rslice()) == 0xBB4E2049);
}

