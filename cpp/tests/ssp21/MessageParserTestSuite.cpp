

#include "catch.hpp"

#include "ssp21/MessageParser.h"

#include <testlib\BufferHelpers.h>

#define SUITE(name) "MessageParserTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("reads integer fields successfully"))
{	
	uint16_t a;
	uint32_t b;

	HexSequence hex("01 00 03 00 00 00");

	auto err = MessageParser::read_fields(hex.as_rslice(), a, b);

	REQUIRE_FALSE(any(err));
	REQUIRE(a == 1);
	REQUIRE(b == 3);
}

TEST_CASE(SUITE("returns error if extra data after fields"))
{
	uint16_t a;
	uint32_t b;

	HexSequence hex("01 00 03 00 00 00 FF");

	auto err = MessageParser::read_fields(hex.as_rslice(), a, b);

	REQUIRE(err == ParseError::too_many_bytes);
}

