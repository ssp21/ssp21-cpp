

#include "catch.hpp"

#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/IntegerField.h"
#include "crypto/MessageParser.h"

#include "ssp21/crypto/SeqByteField.h"
#include "ser4cpp/util/HexConversions.h"

#define SUITE(name) "MessageParserTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

TEST_CASE(SUITE("reads enum values successfully"))
{
    EnumField<FunctionSpec> func;
    auto input = HexConversions::from_hex("00 01");
    auto slice = input->as_rslice();

    REQUIRE(ParseError::ok == func.read(slice));
    REQUIRE(slice.length() == 1);
    REQUIRE(func.value == Function::request_handshake_begin);
}

TEST_CASE(SUITE("reads integer fields values successfully"))
{
    IntegerField<UInt16> value;
    auto input = HexConversions::from_hex("01 02 03");
    auto slice = input->as_rslice();

    REQUIRE(ParseError::ok == value.read(slice));
    REQUIRE(slice.length() == 1);
    REQUIRE(value == 258);
}

TEST_CASE(SUITE("reads integer fields successfully"))
{
    IntegerField<UInt16> a;
    IntegerField<UInt32> b;

    auto input = HexConversions::from_hex("00 01 00 00 00 03");
    auto slice = input->as_rslice();

    auto err = MessageParser::read_fields(slice, a, b);

    REQUIRE_FALSE(any(err));
    REQUIRE(a == 1);
    REQUIRE(b == 3);
}

TEST_CASE(SUITE("returns error if too little data"))
{
    IntegerField<UInt16> a;
    IntegerField<UInt32> b;

    auto input = HexConversions::from_hex("00 01 00 00 00");
    auto slice = input->as_rslice();

    auto err = MessageParser::read_fields(slice, a, b);

    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("ignores extra data after fields"))
{
    IntegerField<UInt16> a;
    IntegerField<UInt32> b;

    auto input = HexConversions::from_hex("00 01 00 00 00 03 FF");
    auto slice = input->as_rslice();

    auto err = MessageParser::read_fields(slice, a, b);

    REQUIRE(err == ParseError::ok);
}

TEST_CASE(SUITE("reads SeqOfByte correctly"))
{
    SeqByteField field;
    auto input = HexConversions::from_hex("04 00 01 02 03 FF");
    auto slice = input->as_rslice();

    auto err = MessageParser::read_fields(slice, field);

    REQUIRE_FALSE(any(err));
    REQUIRE(slice.length() == 1);
    REQUIRE(HexConversions::to_hex(field) == "00 01 02 03");
}

TEST_CASE(SUITE("returns error if SeqOfByte empty"))
{
    SeqByteField field;
    auto slice = seq32_t::empty();
    auto err = MessageParser::read_fields(slice, field);
    REQUIRE(err == ParseError::insufficient_bytes);
}

TEST_CASE(SUITE("returns error if SeqOfByte incomplete"))
{
    SeqByteField field;
    auto input = HexConversions::from_hex("04 00 01 02");
    auto slice = input->as_rslice();

    auto err = MessageParser::read_fields(slice, field);
    REQUIRE(err == ParseError::insufficient_bytes);
}

