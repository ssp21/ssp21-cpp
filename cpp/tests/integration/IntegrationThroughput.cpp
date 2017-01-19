

#include "catch.hpp"

#include "fixtures/IntegrationFixture.h"

#include "ssp21/LogLevels.h"

#define SUITE(name) "IntegrationTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("fixture construction"))
{
    IntegrationFixture fix;
}

TEST_CASE(SUITE("completes handshake"))
{
    IntegrationFixture fix;

    fix.ilog.write_to_stdio();
    //fix.ilog.log_everything();

    fix.responder->on_open();
    fix.initiator->on_open();

    fix.exe->run_one();
}
