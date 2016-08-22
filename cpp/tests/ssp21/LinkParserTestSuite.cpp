

#include "catch.hpp"

#include "ssp21/link/LinkParser.h"

#include "testlib/BufferHelpers.h"
#include "testlib/HexConversions.h"

#define SUITE(name) "LinkParserTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("gracefully handles empty message"))
{				
	LinkParser parser(1024);	

	Hex hex("");
	auto input = hex.as_rslice();
		
	REQUIRE(!parser.parse(input).read_frame);
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("reads a full message properly"))
{	
	LinkParser parser(1024);
	
	Hex hex("07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();

	auto result = parser.parse(input);
	REQUIRE(result.read_frame);
	REQUIRE(result.num_crc_error == 0);
	REQUIRE(input.is_empty());
	
	Addresses addr;
	RSlice payload;
	REQUIRE(parser.read(addr, payload));
	REQUIRE(addr.destination == 1);
	REQUIRE(addr.source == 2);
	REQUIRE(to_hex(payload) == "DD DD DD DD DD DD");

}

TEST_CASE(SUITE("discards leading data properly"))
{
	LinkParser parser(1024);

	Hex hex("FF FF FF FF 07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();

	auto result = parser.parse(input);
	REQUIRE(result.read_frame);
	REQUIRE(result.num_crc_error == 0);
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("detects header crc failure properly"))
{
	LinkParser parser(1024);
	
	/// -----------------------------VV-----------------------------------------
	Hex hex("07 AA 01 00 02 00 06 00 F8 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();

	auto result = parser.parse(input);
	REQUIRE_FALSE(result.read_frame);
	REQUIRE(result.num_crc_error == 1);
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("recursively processes header crc failure"))
{
	LinkParser parser(1024);

	{
		// packet in a packet
		Hex hex("07 AA 07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B");
		auto input = hex.as_rslice();
		auto result = parser.parse(input);
		REQUIRE_FALSE(result.read_frame);
		REQUIRE(result.num_crc_error == 1);
		REQUIRE(input.is_empty());
	}

	{
		// last 3 bytes of body CRC will complete the frame
		Hex trailing("37 0D 51");
		auto input = trailing.as_rslice();
		auto result = parser.parse(input);
		REQUIRE(result.read_frame);
		REQUIRE(result.num_crc_error == 0);
		REQUIRE(input.is_empty());
	}
}

TEST_CASE(SUITE("detects body crc failure properly"))
{
	LinkParser parser(1024);

	/// --------------------------------------------------------------VV--------
	Hex hex("07 AA 01 00 02 00 06 00 F8 9F A2 C3 DD DD DD DD DD DD 6B 38 0D 51");
	auto input = hex.as_rslice();

	auto result = parser.parse(input);
	REQUIRE_FALSE(result.read_frame);
	REQUIRE(result.num_crc_error == 1);
	REQUIRE(input.is_empty());
}
