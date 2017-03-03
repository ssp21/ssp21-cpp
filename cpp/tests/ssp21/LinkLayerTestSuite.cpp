

#include "catch.hpp"

#include "fixtures/LinkLayerFixture.h"
#include "mocks/HexMessageBuilders.h"

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

TEST_CASE(SUITE("processes a valid frame"))
{
    LinkLayerFixture fix;
    fix.link.on_open();
    fix.lower.enqueue_message(hex::link_frame(10, 1, "CA FE"));
    REQUIRE(fix.upper.pop_rx_message() == "CA FE");
}

