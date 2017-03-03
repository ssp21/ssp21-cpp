

#include "catch.hpp"

#include "fixtures/LinkLayerFixture.h"

#define SUITE(name) "LinkLayerTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("opens upper when opened"))
{
    LinkLayerFixture fix;
	
	REQUIRE_FALSE(fix.upper.get_is_open());
	fix.link.on_open();
	REQUIRE(fix.upper.get_is_open());
}

