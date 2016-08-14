

#include "catch.hpp"
#include "ssp21/Crypto.h"

#include "testlib/HexConversions.h"

#include <cstring>

#define SUITE(name) "SHA256TestSuite - " name

using namespace ssp21;

TEST_CASE(SUITE("sha256"))
{
	std::string text("The quick brown fox");

	auto slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());

	SymmetricKey output;
	Crypto::hash_sha256({slice}, output);
	REQUIRE(output.get_type() == BufferType::sha256);

	auto hex = testlib::ToHex(output.as_slice(), false);
	REQUIRE(hex == "5CAC4F980FEDC3D3F1F99B4BE3472C9B30D56523E632D151237EC9309048BDA9");	
}

TEST_CASE(SUITE("HMAC-sha256"))
{
	std::string text("The quick brown fox");
	std::string key("somesecret");

	auto text_slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());
	auto key_slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());

	SymmetricKey output;
	Crypto::hmac_sha256(key_slice, { text_slice }, output);
	REQUIRE(output.get_type() == BufferType::sha256);

	auto hex = testlib::ToHex(output.as_slice(), false);
	REQUIRE(hex == "9F93EAF321335A7F3B4F9FBB872123F37E51F494F4062D32588295FEEDB08F82");
}

// TODO: Find a proper test vector. The RFC formulates things differently. See what noise implementations do.
// for now, we'll just run it and check that it doesn't blow up.
TEST_CASE(SUITE("HKDF-sha256"))
{
	std::string secret1("The quick brown fox");
	std::string secret2("somesecret");

	auto secret1_slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(secret1.c_str()), secret1.size());
	auto secret2_slice = openpal::RSlice(reinterpret_cast<const uint8_t*>(secret2.c_str()), secret1.size());

	SymmetricKey key1;
	SymmetricKey key2;
	
	Crypto::hkdf_sha256(secret1_slice, { secret2_slice }, key1, key2);
	REQUIRE(key1.get_type() == BufferType::symmetric_key);
	REQUIRE(key2.get_type() == BufferType::symmetric_key);

}


