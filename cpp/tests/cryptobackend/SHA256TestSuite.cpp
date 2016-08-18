

#include "catch.hpp"
#include "ssp21/Crypto.h"

#include "testlib/HexConversions.h"
#include "testlib/BufferHelpers.h"

#include <cstring>

#define SUITE(name) "SHA256TestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("sha256"))
{
	std::string text("The quick brown fox");

	auto slice = RSlice(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());

	HashOutput output;
	Crypto::hash_sha256({slice}, output);
	REQUIRE(output.get_type() == BufferType::sha256);

	auto hex = to_hex(output.as_slice(), false);
	REQUIRE(hex == "5CAC4F980FEDC3D3F1F99B4BE3472C9B30D56523E632D151237EC9309048BDA9");	
}

TEST_CASE(SUITE("sha256 can safely be invoked on its own output buffer"))
{
	std::string text("The quick brown fox");
	auto slice = RSlice(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());

	HashOutput output;
	Crypto::hash_sha256({ slice }, output);
	
	// now mix the hash
	Crypto::hash_sha256({ output.as_slice(), slice }, output);

	auto hex = to_hex(output.as_slice(), false);
	REQUIRE(hex == "A40F5F6A51B38D29F07846CE1753042920C0F8E871BBCBDDC99B785D2C4C25F7");
}

TEST_CASE(SUITE("HMAC-sha256"))
{
	std::string text("The quick brown fox");
	std::string key("somesecret");

	auto text_slice = RSlice(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());
	auto key_slice = RSlice(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());

	HashOutput output;
	Crypto::hmac_sha256(key_slice, { text_slice }, output);
	REQUIRE(output.get_type() == BufferType::sha256);

	auto hex = to_hex(output.as_slice(), false);
	REQUIRE(hex == "9F93EAF321335A7F3B4F9FBB872123F37E51F494F4062D32588295FEEDB08F82");
}

/**
*
* Modified the test code from the Rust implementation of RFC 5869 below to produce
* a suitable test vector.
*
* https://github.com/wireapp/hkdf
*/
TEST_CASE(SUITE("HKDF-sha256"))
{
	HexSequence ikm("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b");
	HexSequence salt("deadbeef");	

	SymmetricKey key1;
	SymmetricKey key2;
	
	Crypto::hkdf_sha256(salt.as_rslice(), { ikm.as_rslice() }, key1, key2);
	
	REQUIRE(key1.get_type() == BufferType::symmetric_key);
	REQUIRE(key2.get_type() == BufferType::symmetric_key);

	auto hex_key_1 = to_hex(key1.as_slice(), false);
	auto hex_key_2 = to_hex(key2.as_slice(), false);

	REQUIRE(hex_key_1 == "848036BCEE099D03B956D92DE8C41A2345887836760EA09CE15885E8CAA7444C");
	REQUIRE(hex_key_2 == "5A72EE93D8AE3FF67C25680A02AD5C99DC2E6CA2ADBB85A251809EE4A76C6EF5");
}


