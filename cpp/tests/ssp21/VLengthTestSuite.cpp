

#include "catch.hpp"

#include "ssp21/crypto/VLength.h"

#include "openpal/container/StaticBuffer.h"
#include "testlib/HexConversions.h"
#include "testlib/Hex.h"

#define SUITE(name) "VLenthTestSuite - " name

using namespace ssp21;
using namespace openpal;

void test_encoding_decoding(uint32_t number, const std::string& hex)
{
    StaticBuffer<uint32_t, 5> buffer;
    auto dest = buffer.as_wseq();

    REQUIRE(VLength::write(number, dest) == FormatError::ok);
    const auto length = buffer.size() - dest.length();

    //ensure that the size written is what the size_in_bytes() function tells us it would be
    REQUIRE(length == VLength::size_in_bytes(number));

    const auto bytes = buffer.as_seq(length);
    REQUIRE(to_hex(bytes) == hex);

    auto remainder = bytes;
    uint32_t parsed_value;
    REQUIRE(VLength::read(parsed_value, remainder) == ParseError::ok);
    REQUIRE(parsed_value == number);
}

void test_decoding_failure(const std::string& data, ParseError expected_err)
{
    Hex hex(data);
    auto remainder = hex.as_rslice();
    uint32_t parsed_value;

    const auto err = VLength::read(parsed_value, remainder);
    REQUIRE(remainder.is_empty());
    REQUIRE(err == expected_err);
}

TEST_CASE(SUITE("encodes max and  min"))
{
    test_encoding_decoding(0, "00");
    test_encoding_decoding(std::numeric_limits<uint32_t>::max(), "FF FF FF FF 0F");
}

TEST_CASE(SUITE("encodes at one byte boundary"))
{
    test_encoding_decoding(127, "7F");
    test_encoding_decoding(128, "80 01");
}

TEST_CASE(SUITE("encodes at two byte boundary"))
{
    test_encoding_decoding(16383, "FF 7F");
    test_encoding_decoding(16384, "80 80 01");
}

TEST_CASE(SUITE("encodes at three byte boundary"))
{
    test_encoding_decoding(2097151, "FF FF 7F");
    test_encoding_decoding(2097152, "80 80 80 01");
}

TEST_CASE(SUITE("encodes at four byte boundary"))
{
    test_encoding_decoding(268435455, "FF FF FF 7F");
    test_encoding_decoding(268435456, "80 80 80 80 01");
}

TEST_CASE(SUITE("decoding fails when fewer bytes could have been used to encode value"))
{
    test_decoding_failure("80 00", ParseError::bad_vlength);
    test_decoding_failure("80 80 00", ParseError::bad_vlength);
    test_decoding_failure("80 80 80 00", ParseError::bad_vlength);
    test_decoding_failure("80 80 80 80 00", ParseError::bad_vlength);
}

TEST_CASE(SUITE("decoding fails when maximum value is exceeded on the last byte"))
{
    test_decoding_failure("FF FF FF FF 10", ParseError::bad_vlength);
}

TEST_CASE(SUITE("decoding fails when insufficient bytes"))
{
    test_decoding_failure("", ParseError::insufficient_bytes);
    test_decoding_failure("80", ParseError::insufficient_bytes);
    test_decoding_failure("80 80", ParseError::insufficient_bytes);
    test_decoding_failure("80 80 80", ParseError::insufficient_bytes);
    test_decoding_failure("80 80 80 80", ParseError::insufficient_bytes);
}

