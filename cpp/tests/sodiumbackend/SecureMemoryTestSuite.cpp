

#include "catch.hpp"

#include "ssp21/Crypto.h"

#define SUITE(name) "SecureMemoryTestSuite - " name

using namespace ssp21;

// not testing for constant time here, just that the 
// binding to sodium is correct...
TEST_CASE(SUITE("test secure comparison"))
{
	uint8_t buff1[3] = { 0x01, 0x02, 0x03 };
	uint8_t buff2[3] = { 0x01, 0x02, 0x04 };	
	
	auto slice1 = openpal::RSlice(buff1, 3);
	auto slice2 = openpal::RSlice(buff2, 3);

	REQUIRE(Crypto::secure_equals(slice1, slice1));
	REQUIRE_FALSE(Crypto::secure_equals(slice1, slice2));
}

