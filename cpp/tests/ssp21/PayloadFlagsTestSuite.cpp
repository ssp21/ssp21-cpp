

#include "catch.hpp"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#include "ssp21/msg/PayloadFlags.h"
#include "ssp21/crypto/LogMessagePrinter.h"

#include "mocks/MockLogHandler.h"

#define SUITE(name) "PayloadFlagsTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("Correct defaults"))
{
	PayloadFlags flags;
	REQUIRE(flags.fir);
	REQUIRE(flags.fin);
}

TEST_CASE(SUITE("Correct serialization"))
{		
	auto test_permutation = [](const PayloadFlags& flags, uint8_t expected)
	{
		uint8_t value = 0;
		auto dest = openpal::WSlice(&value, 1);
		auto err = flags.write(dest);
		REQUIRE(!any(err));
		REQUIRE(dest.is_empty());
		REQUIRE(value == expected);
	};

	test_permutation(PayloadFlags(false, false), 0x00);
	test_permutation(PayloadFlags(true, false), 0x80);
	test_permutation(PayloadFlags(false, true), 0x40);
	test_permutation(PayloadFlags(true, true), 0xC0);
}

TEST_CASE(SUITE("Correct deserialization"))
{
	auto test_permutation = [](bool fir, bool fin, uint8_t byte)
	{
		auto input = openpal::RSlice(&byte, 1);
		PayloadFlags flags;		
		REQUIRE(!any(flags.read(input)));
		REQUIRE(input.is_empty());
		REQUIRE(flags.fir == fir);
		REQUIRE(flags.fin == fin);
	};

	test_permutation(false, false, 0x00);
	test_permutation(true, false, 0x80);
	test_permutation(false, true, 0x40);
	test_permutation(true, true, 0xC0);
}

TEST_CASE(SUITE("error if reserved bit is set"))
{
		uint8_t byte = 0x01;
	
		auto input = openpal::RSlice(&byte, 1);
		PayloadFlags flags;
		auto err = flags.read(input);
		REQUIRE(err == ParseError::reserved_bit);
}

TEST_CASE(SUITE("pretty printing"))
{	
	PayloadFlags flags(true, false);

	MockLogHandler log("log");
	LogMessagePrinter printer(log.root.logger, ssp21::levels::info, 16);

	flags.print("flags", printer);
	
	log.expect(
		"flags { fir: 1 fin: 0 }"		
	);
}

