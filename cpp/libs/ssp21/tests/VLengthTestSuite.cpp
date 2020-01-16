

#include "catch.hpp"

#include "ssp21/crypto/VLength.h"

#include "ser4cpp/container/StaticBuffer.h"
#include "ser4cpp/util/HexConversions.h"

#define SUITE(name) "VLenthTestSuite - " name

using namespace ssp21;
using namespace ser4cpp;

void test_encoding_decoding(uint32_t number, const std::string& hex)
{
    StaticBuffer<uint32_t, 5> buffer;
    auto dest = buffer.as_wseq();

    REQUIRE(VLength::write(number, dest) == FormatError::ok);
    const auto length = buffer.length() - dest.length();

    //ensure that the size written is what the size_in_bytes() function tells us it would be
    REQUIRE(length == VLength::size(number));

    const auto bytes = buffer.as_seq(length);
    REQUIRE(HexConversions::to_hex(bytes) == hex);

    auto remainder = bytes;
    uint32_t parsed_value;
    REQUIRE(VLength::read(parsed_value, remainder) == ParseError::ok);
    REQUIRE(parsed_value == number);
}

void test_decoding_failure(const std::string& data, ParseError expected_err)
{
    auto input = HexConversions::from_hex(data);
    auto slice = input->as_rslice();
    uint32_t parsed_value;

    const auto err = VLength::read(parsed_value, slice);
    REQUIRE(err == expected_err);
}

TEST_CASE(SUITE("encodes max and  min"))
{
    test_encoding_decoding(0, "00");
    test_encoding_decoding(std::numeric_limits<uint32_t>::max(), "84 FF FF FF FF");
}

TEST_CASE(SUITE("encodes at one byte boundary"))
{
    test_encoding_decoding(127, "7F");
    test_encoding_decoding(128, "81 80");
}

TEST_CASE(SUITE("encodes at two byte boundary"))
{
    test_encoding_decoding(65535, "82 FF FF");
    test_encoding_decoding(65536, "83 01 00 00");
}

TEST_CASE(SUITE("encodes at three byte boundary"))
{
    test_encoding_decoding(16777215, "83 FF FF FF");
    test_encoding_decoding(16777216, "84 01 00 00 00");
}

TEST_CASE(SUITE("byte counts not in [1, 4] fail"))
{
    test_decoding_failure("80", ParseError::bad_vlength);
    test_decoding_failure("85", ParseError::bad_vlength);
}

TEST_CASE(SUITE("decoding fails when fewer bytes could have been used to encode value"))
{
    test_decoding_failure("81 7F", ParseError::bad_vlength);
    test_decoding_failure("82 00 FF", ParseError::bad_vlength);
    test_decoding_failure("83 00 FF FF", ParseError::bad_vlength);
    test_decoding_failure("84 00 FF FF FF", ParseError::bad_vlength);
}

TEST_CASE(SUITE("decoding fails when insufficient bytes"))
{
    test_decoding_failure("", ParseError::insufficient_bytes);
    test_decoding_failure("81", ParseError::insufficient_bytes);
    test_decoding_failure("82 00", ParseError::insufficient_bytes);
    test_decoding_failure("83 00 00", ParseError::insufficient_bytes);
    test_decoding_failure("84 00 00 00", ParseError::insufficient_bytes);
}
