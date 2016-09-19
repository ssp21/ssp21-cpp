

#include "catch.hpp"

#include "ssp21/crypto/Base64.h"
#include "testlib/Hex.h"

#define SUITE(name) "Base64TestSuite - " name

using namespace ssp21;
using namespace openpal;

void test_encoding(const std::string& input, const std::string& expected_output)
{
    std::vector<char> output;
    auto write = [&](char c)
    {
        output.push_back(c);
    };

    openpal::RSlice slice(reinterpret_cast<const uint8_t*>(input.c_str()), static_cast<uint32_t>(input.length()));

    Base64::encode(slice, write);

    std::string output_string(output.begin(), output.end());


    REQUIRE(output_string == expected_output);
}

void test_decoding_failure(const std::string& input, Base64DecodeError expected_err)
{
    std::vector<uint8_t> output;
    auto write = [&](char c)
    {
        output.push_back(c);
    };

    openpal::RSlice slice(reinterpret_cast<const uint8_t*>(input.c_str()), static_cast<uint32_t>(input.length()));

    auto err = Base64::decode(slice, write);

    REQUIRE(err == expected_err);
}

void test_decoding_success(const std::string& input, const std::string& expected_output)
{
    std::vector<char> output;
    auto write = [&](char c)
    {
        output.push_back(c);
    };

    openpal::RSlice slice(reinterpret_cast<const uint8_t*>(input.c_str()), static_cast<uint32_t>(input.length()));

    REQUIRE(Base64::decode(slice, write) == Base64DecodeError::ok);

    std::string output_string(output.begin(), output.end());

    REQUIRE(output_string == expected_output);
}

/// -------- encoding --------

TEST_CASE(SUITE("correctly encodes empty string"))
{
    test_encoding("", "");
}

TEST_CASE(SUITE("correctly encodes even multiple of three"))
{
    test_encoding("ManMan", "TWFuTWFu");
}

TEST_CASE(SUITE("correctly encodes module 1"))
{
    test_encoding("ManM", "TWFuTQ==");
}

TEST_CASE(SUITE("correctly encodes module 2"))
{
    test_encoding("ManMa", "TWFuTWE=");
}

/// -------- decoding --------

TEST_CASE(SUITE("rejects bad input size"))
{
    test_decoding_failure("TQ=", Base64DecodeError::not_mult_four);
}

TEST_CASE(SUITE("rejects trailing bytes"))
{
    test_decoding_failure("TQ==TWFu", Base64DecodeError::bad_trailing_input);
}

TEST_CASE(SUITE("rejects bad characters"))
{
    test_decoding_failure("TQ!=", Base64DecodeError::not_base64);
}

TEST_CASE(SUITE("correctly decodes one byte"))
{
    test_decoding_success("TQ==", "M");
}

TEST_CASE(SUITE("skips leading and trailing whitespace after termination characters"))
{
    test_decoding_success("\r\nT Q =\t=\n\r\t ", "M");
}

TEST_CASE(SUITE("correctly decodes two bytes"))
{
    test_decoding_success("TWE=", "Ma");
}

TEST_CASE(SUITE("correctly decodes three bytes"))
{
    test_decoding_success("TWFu", "Man");
}

TEST_CASE(SUITE("skips trailing whitespace after non-terminating characters"))
{
	test_decoding_success("TWFu\n\r\t", "Man");
}

TEST_CASE(SUITE("correctly decodes six bytes"))
{
    test_decoding_success("TWFuTQ==", "ManM");
}

TEST_CASE(SUITE("correctly decodes long input"))
{
    auto input = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGl"
				  "zIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
                  "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlud"
                  "WVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZS"
                  "BzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

	auto result = "Man is distinguished, not only by his reason, but by this singular passion from "
                  "other animals, which is a lust of the mind, that by a perseverance of delight in the "
                  "continued and indefatigable generation of knowledge, exceeds the short vehemence of "
                  "any carnal pleasure.";

    test_decoding_success(input, result);
}