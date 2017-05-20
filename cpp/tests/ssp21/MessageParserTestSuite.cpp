

#include "catch.hpp"

#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/MessageParser.h"

#include "ssp21/crypto/SeqByteField.h"

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

TEST_CASE(SUITE("reads integer fields values successfully"))
{
    IntegerField<UInt16> value;
    Hex hex("01 02 03");

    auto input = hex.as_rslice();

    REQUIRE(ParseError::ok == value.read(input));
    REQUIRE(input.length() == 1);
    REQUIRE(value == 258);
}

TEST_CASE(SUITE("reads integer fields successfully"))
{
    IntegerField<UInt16> a;
    IntegerField<UInt32> b;

    Hex hex("00 01 00 00 00 03");

    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, a, b);

    REQUIRE_FALSE(any(err));
    REQUIRE(a == 1);
    REQUIRE(b == 3);
}

TEST_CASE(SUITE("returns error if too little data"))
{
    IntegerField<UInt16> a;
    IntegerField<UInt32> b;

    Hex hex("00 01 00 00 00");

    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, a, b);

    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("ignores extra data after fields"))
{
    IntegerField<UInt16> a;
    IntegerField<UInt32> b;

    Hex hex("00 01 00 00 00 03 FF");

    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, a, b);

    REQUIRE(err == ParseError::ok);
}

TEST_CASE(SUITE("reads Seq8 correctly"))
{
    SeqByteField<openpal::UInt8> field;
    Hex hex("04 00 01 02 03 FF");

    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, field);

    REQUIRE_FALSE(any(err));
    REQUIRE(input.length() == 1);
    REQUIRE(to_hex(field) == "00 01 02 03");
}

TEST_CASE(SUITE("returns error if Seq8 empty"))
{
    SeqByteField<openpal::UInt8> field;
    auto input = seq32_t::empty();
    auto err = MessageParser::read_fields(input, field);
    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("returns error if Seq8 incomplete"))
{
    SeqByteField<openpal::UInt8> field;
    Hex hex("04 00 01 02");
    auto input = hex.as_rslice();
    auto err = MessageParser::read_fields(input, field);
    REQUIRE(err == ParseError::insufficient_bytes);
}

