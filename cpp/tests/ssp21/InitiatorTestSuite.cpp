
#include "catch.hpp"

#include "fixtures/CryptoLayerFixture.h"

#define SUITE(name) "InitiatorTestSuite - " name

using namespace ssp21;
using namespace openpal;

// ---------- tests for handshake state idle -----------

TEST_CASE(SUITE("construction"))
{
    InitiatorFixture fix;
}

