

#include "catch.hpp"

#include "fixtures/IntegrationFixture.h"

#define SUITE(name) "IntegrationTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("fixture construction"))
{
    IntegrationFixture fix;
}
