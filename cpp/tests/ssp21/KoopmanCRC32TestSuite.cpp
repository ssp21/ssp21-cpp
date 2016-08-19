

#include "catch.hpp"

#include "ssp21/link/KoopmanCRC32.h"
#include "testlib/BufferHelpers.h"

#define SUITE(name) "KoopmanCRC32TestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("test vector 1"))
{		
	HexSequence hex("31 32 33 34 35 36 37 38 39");
	REQUIRE(CRC32Koopman::calc(hex.as_rslice()) == 0x9601DDA9);
}

TEST_CASE(SUITE("test vector 2"))
{
	HexSequence hex("DE AD BE EF");
	REQUIRE(CRC32Koopman::calc(hex.as_rslice()) == 0xFDBCA758);
}