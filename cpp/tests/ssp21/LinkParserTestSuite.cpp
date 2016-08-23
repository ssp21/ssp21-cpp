

#include "catch.hpp"

#include "ssp21/link/LinkParser.h"

#include "testlib/BufferHelpers.h"
#include "testlib/HexConversions.h"

#define SUITE(name) "LinkParserTestSuite - " name

using namespace ssp21;
using namespace openpal;

class CountingReporter : public LinkParser::IReporter
{

public:

	virtual void on_bad_header_crc(uint32_t expected, uint32_t actual) override
	{
		++num_bad_header_crc;
	}

	virtual void on_bad_body_crc(uint32_t expected, uint32_t actual) override
	{
		++num_bad_body_crc;
	}

	virtual void on_bad_body_length(uint32_t max_allowed, uint32_t actual) override
	{
		++num_bad_length;
	}

	bool no_errors() const
	{
		return (num_bad_header_crc | num_bad_body_crc | num_bad_length) == 0;
	}

	void clear()
	{
		num_bad_header_crc = num_bad_body_crc = num_bad_length = 0;
	}


	uint32_t num_bad_header_crc = 0;
	uint32_t num_bad_body_crc = 0;
	uint32_t num_bad_length = 0;
};

TEST_CASE(SUITE("gracefully handles empty message"))
{				
	CountingReporter reporter;
	LinkParser parser(1024, reporter);

	Hex hex("");
	auto input = hex.as_rslice();
		
	REQUIRE(!parser.parse(input));
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("reads a full message properly"))
{	
	CountingReporter reporter;
	LinkParser parser(1024, reporter);
	
	Hex hex("07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();
	
	REQUIRE(parser.parse(input));
	REQUIRE(reporter.no_errors());
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
	CountingReporter reporter;
	LinkParser parser(1024, reporter);

	Hex hex("FF FF FF FF 07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();

	REQUIRE(parser.parse(input));	
	REQUIRE(reporter.no_errors());
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("detects header crc failure properly"))
{
	CountingReporter reporter;
	LinkParser parser(1024, reporter);
	
	/// -----------------------------VV-----------------------------------------
	Hex hex("07 AA 01 00 02 00 06 00 F8 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();
	
	REQUIRE_FALSE(parser.parse(input));
	REQUIRE(reporter.num_bad_header_crc == 1);
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("recursively processes header crc failure"))
{
	CountingReporter reporter;
	LinkParser parser(1024, reporter);

	{
		// packet in a packet
		//             07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51 < - valid packet
		Hex hex("07 AA 07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B");
		auto input = hex.as_rslice();
		
		REQUIRE_FALSE(parser.parse(input));
		REQUIRE(reporter.num_bad_header_crc == 1);
		REQUIRE(input.is_empty());
	}

	reporter.clear();

	{
		// last 3 bytes of body CRC will complete the frame
		Hex trailing("37 0D 51");
		auto input = trailing.as_rslice();
		
		REQUIRE(parser.parse(input));
		REQUIRE(reporter.no_errors());
		REQUIRE(input.is_empty());
	}
}

TEST_CASE(SUITE("detects body crc failure properly"))
{
	CountingReporter reporter;
	LinkParser parser(1024, reporter);

		
	/// --------------------------------------------------------------VV--------
	Hex hex("07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 38 0D 51");
	auto input = hex.as_rslice();
	
	REQUIRE_FALSE(parser.parse(input));
	REQUIRE(reporter.num_bad_body_crc == 1);
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("allows frame with maximum body length"))
{
	CountingReporter reporter;
	LinkParser parser(6, reporter);

	/// -----------------------LL-LL--------------------------------------------
	Hex hex("07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();

	REQUIRE(parser.parse(input));
	REQUIRE(reporter.no_errors());
	REQUIRE(input.is_empty());
}

TEST_CASE(SUITE("fails if length exceeds maximum body length"))
{
	CountingReporter reporter;
	LinkParser parser(5, reporter);

	/// -----------------------LL-LL--------------------------------------------
	Hex hex("07 AA 01 00 02 00 06 00 F9 9F A2 C3 DD DD DD DD DD DD 6B 37 0D 51");
	auto input = hex.as_rslice();

	REQUIRE_FALSE(parser.parse(input));
	REQUIRE(reporter.num_bad_length == 1);
	REQUIRE(reporter.num_bad_header_crc == 0);
	REQUIRE(reporter.num_bad_body_crc == 0);	
	REQUIRE(input.is_empty());
}