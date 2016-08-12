

#include "catch.hpp"
#include "ssp21/Crypto.h"

#include "testlib/HexConversions.h"

#include <cstring>

#define SUITE(name) "SHA256TestSuite - " name

using namespace ssp21;

TEST_CASE(SUITE("SHA256"))
{
	std::string text("The quick brown fox");

	auto slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());

	HashOutput output;
	Crypto::hash_sha256({slice}, output);
	REQUIRE(output.get_type() == BufferType::SHA256);

	auto hex = testlib::ToHex(output.as_slice(), false);
	REQUIRE(hex == "5CAC4F980FEDC3D3F1F99B4BE3472C9B30D56523E632D151237EC9309048BDA9");	
}

TEST_CASE(SUITE("HMAC-SHA256"))
{
	std::string text("The quick brown fox");
	std::string key("somesecret");

	auto text_slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());
	auto key_slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());

	HashOutput output;
	Crypto::hmac_sha256(key_slice, { text_slice }, output);
	REQUIRE(output.get_type() == BufferType::SHA256);

	auto hex = testlib::ToHex(output.as_slice(), false);
	REQUIRE(hex == "9F93EAF321335A7F3B4F9FBB872123F37E51F494F4062D32588295FEEDB08F82");
}


