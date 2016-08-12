

#include "catch.hpp"
#include "ssp21/Crypto.h"

#include "testlib/HexConversions.h"

#include <cstring>

#define SUITE(name) "SHA256TestSuite - " name

using namespace ssp21;

TEST_CASE(SUITE("SHA256 output"))
{
	auto text = "The quick brown fox";

	auto slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(text), strlen(text));

	HashOutput output;
	Crypto::hash_sha256({slice}, output);
	REQUIRE(output.get_type() == HashOutputType::SHA256);

	auto hex = testlib::ToHex(output.as_slice(), false);
	REQUIRE(hex == "5CAC4F980FEDC3D3F1F99B4BE3472C9B30D56523E632D151237EC9309048BDA9");
	
}


