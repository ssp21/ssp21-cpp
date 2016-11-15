
#include "catch.hpp"

#include "ssp21/crypto/Reassembler.h"

#include "testlib/Hex.h"
#include "testlib/HexConversions.h"

#define SUITE(name) "ReassemblerTestSuite - " name

using namespace ssp21;
using namespace openpal;

void test_complete_reassembly(Reassembler& reassembler, bool fir, bool fin, uint32_t nonce, const std::string& segment, const std::string& output);
void test_incomplete_reassembly(Reassembler& reassembler, bool fir, bool fin, uint32_t nonce, const std::string& segment, ReassemblyResult expected);



// ---------- tests for single unit messages -----------

TEST_CASE(SUITE("rejects single segment message w/ nonce of zero"))
{
    Reassembler reassembler(1);
    test_incomplete_reassembly(reassembler, true, true, 0, "01", ReassemblyResult::bad_nonce);
}

TEST_CASE(SUITE("accepts single segment message"))
{
    Reassembler reassembler(1);
    const auto input = "01";
    test_complete_reassembly(reassembler, true, true, 1, input, input);
}

TEST_CASE(SUITE("rejects single segment overflow"))
{
    Reassembler reassembler(1);
    test_incomplete_reassembly(reassembler, true, true, 1, "01 02", ReassemblyResult::overflow);
}

// ---------- tests for multi unit messages -----------

TEST_CASE("accepts three segment message")
{
    Reassembler reassembler(4);
    test_incomplete_reassembly(reassembler, true, false, 1, "01", ReassemblyResult::partial);
    test_incomplete_reassembly(reassembler, false, false, 2, "02", ReassemblyResult::partial);
    test_complete_reassembly(reassembler, false, true, 3, "03 04", "01 02 03 04");
}

TEST_CASE("detects overflow in multi segment message")
{
	Reassembler reassembler(4);
	test_incomplete_reassembly(reassembler, true, false, 1, "01", ReassemblyResult::partial);
	test_incomplete_reassembly(reassembler, false, false, 2, "02", ReassemblyResult::partial);
	test_incomplete_reassembly(reassembler, false, true, 3, "03 04 05", ReassemblyResult::overflow);
}

TEST_CASE("detects discontiguous nonce")
{
	Reassembler reassembler(2);
	test_incomplete_reassembly(reassembler, true, false, 1, "01", ReassemblyResult::partial);
	test_incomplete_reassembly(reassembler, false, false, 3, "02", ReassemblyResult::bad_nonce);
}

TEST_CASE("discards data when new fir arrives")
{
	Reassembler reassembler(2);
	test_incomplete_reassembly(reassembler, true, false, 1, "01", ReassemblyResult::partial);
	test_complete_reassembly(reassembler, true, true, 3, "03 04", "03 04");
}

// ---------- helper method impls -----------

void test_complete_reassembly(Reassembler& reassembler, bool fir, bool fin, uint32_t nonce, const std::string& segment, const std::string& output)
{
    Hex input(segment);
    const auto result = reassembler.process(fir, fin, nonce, input.as_rslice());
    REQUIRE(result == ReassemblyResult::complete);
    REQUIRE(to_hex(reassembler.get_data()) == output);
}

void test_incomplete_reassembly(Reassembler& reassembler, bool fir, bool fin, uint32_t nonce, const std::string& segment, ReassemblyResult expected)
{
    Hex input(segment);
    const auto result = reassembler.process(fir, fin, nonce, input.as_rslice());
    REQUIRE(result == expected);
}