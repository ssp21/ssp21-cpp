

#include "catch.hpp"

#include "fixtures/LinkLayerFixture.h"

#define SUITE(name) "LinkLayerTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("construction"))
{
    LinkLayerFixture fix;
}

