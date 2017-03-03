

#include "catch.hpp"

#include "ssp21/link/LinkLayer.h"

#define SUITE(name) "LinkLayerTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("construction"))
{
    LinkLayer link(1, 10);
}

