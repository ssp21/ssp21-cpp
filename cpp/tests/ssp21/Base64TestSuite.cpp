

#include "catch.hpp"

#include "ssp21/crypto/Base64.h"
#include "testlib/Hex.h"

#define SUITE(name) "Base64TestSuite - " name

using namespace ssp21;
using namespace openpal;

void test_encoding(const std::string& input, const std::string& expected_output)
{
	std::vector<char> output;
	auto write = [&](char c) { output.push_back(c); };

	openpal::RSlice slice(reinterpret_cast<const uint8_t*>(input.c_str()), static_cast<uint32_t>(input.length()));

	Base64::encode(slice, write);

	std::string output_string(output.begin(), output.end());
	

	REQUIRE(output_string == expected_output);
}

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