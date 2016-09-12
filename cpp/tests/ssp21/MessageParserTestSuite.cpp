

#include "catch.hpp"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/SequenceTypes.h"
#include "ssp21/crypto/EnumField.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#define SUITE(name) "MessageParserTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("reads enum values successfully"))
{
	EnumField<FunctionSpec> func;
	Hex hex("00 01");

	auto input = hex.as_rslice();

	REQUIRE(ParseError::ok == func.read(input));
	REQUIRE(input.length() == 1);
	REQUIRE(func.value == Function::request_handshake_begin);
}

TEST_CASE(SUITE("reads integer fields successfully"))
{
    uint16_t a;
    uint32_t b;

    Hex hex("00 01 00 00 00 03");

    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, a, b);

    REQUIRE_FALSE(any(err));
    REQUIRE(a == 1);
    REQUIRE(b == 3);
}

TEST_CASE(SUITE("returns error if too little data"))
{
    uint16_t a;
    uint32_t b;

    Hex hex("00 01 00 00 00");

    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, a, b);

    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("ignores extra data after fields"))
{
    uint16_t a;
    uint32_t b;

    Hex hex("00 01 00 00 00 03 FF");

    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, a, b);

    REQUIRE(err == ParseError::ok);
}

TEST_CASE(SUITE("reads Seq8 correctly"))
{
    Seq8 seq;
    Hex hex("04 00 01 02 03 FF");

    auto input = hex.as_rslice();
    auto err = MessageParser::read(input, seq);

    REQUIRE_FALSE(any(err));
    REQUIRE(input.length() == 1);
    REQUIRE(to_hex(seq) == "00 01 02 03");
}

TEST_CASE(SUITE("returns error if Seq8 empty"))
{
    Seq8 seq;
    auto input = RSlice::empty_slice();
    auto err = MessageParser::read(input, seq);
    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("returns error if Seq8 incomplete"))
{
    Seq8 seq;
    Hex hex("04 00 01 02");
    auto input = hex.as_rslice();
    auto err = MessageParser::read(input, seq);
    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("reads Seq8Seq16 correctly"))
{
    Seq8Seq16 seqs;
    Hex hex("02 00 01 BB 00 02 CA FE DD");
    auto input = hex.as_rslice();
    auto err = MessageParser::read(input, seqs);
    REQUIRE(!any(err));
    REQUIRE(to_hex(input) == "DD"); // remainder
    REQUIRE(seqs.count() == 2);


    Seq16 slice;
    REQUIRE(seqs.read(0, slice));
    REQUIRE(to_hex(slice) == "BB");
    REQUIRE(seqs.read(1, slice));
    REQUIRE(to_hex(slice) == "CA FE");
    REQUIRE_FALSE(seqs.read(2, slice));
}

TEST_CASE(SUITE("returns err if Seq8Seq16 reaches limit"))
{
    Seq8Seq16 seqs;
    Hex hex("07 00 01 00 00 01 00 00 01 00 00 01 00 00 01 00");
    auto input = hex.as_rslice();
    auto err = MessageParser::read(input, seqs);
    REQUIRE(err == ParseError::impl_capacity_limit);
}
