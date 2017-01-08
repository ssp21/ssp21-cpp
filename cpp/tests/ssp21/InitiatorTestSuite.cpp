
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

TEST_CASE(SUITE("transmits REQUEST_HANDSHAKE_BEGIN when opened"))
{
    InitiatorFixture fix;
    REQUIRE(fix.lower.num_tx_messages() == 0);
}

