

#include "catch.hpp"

#include "ssp21/MessageParser.h"

#include <testlib\BufferHelpers.h>
#include <testlib\HexConversions.h>

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

TEST_CASE(SUITE("returns error if too little data"))
{
	uint16_t a;
	uint32_t b;

	HexSequence hex("01 00 03 00 00");

	auto err = MessageParser::read_fields(hex.as_rslice(), a, b);

	REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("returns error if extra data after fields"))
{
	uint16_t a;
	uint32_t b;

	HexSequence hex("01 00 03 00 00 00 FF");

	auto err = MessageParser::read_fields(hex.as_rslice(), a, b);

	REQUIRE(err == ParseError::too_many_bytes);
}

TEST_CASE(SUITE("reads Seq8 correctly"))
{
	Seq8 seq;
	HexSequence hex("04 00 01 02 03 FF");

	auto input = hex.as_rslice();
	auto err = MessageParser::read(input, seq);

	REQUIRE_FALSE(any(err));
	REQUIRE(input.length() == 1);
	REQUIRE(to_hex(seq) == "00 01 02 03");
}

TEST_CASE(SUITE("returns error if Seq8 empty"))
{
	Seq8 seq;		
	auto err = MessageParser::read(RSlice::empty_slice(), seq);
	REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("returns error if Seq8 incomplete"))
{
	Seq8 seq;
	HexSequence hex("04 00 01 02");
	auto err = MessageParser::read(RSlice::empty_slice(), seq);
	REQUIRE(err == ParseError::insufficient_bytes);
}

