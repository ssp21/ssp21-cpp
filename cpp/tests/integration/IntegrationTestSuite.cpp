

#include "catch.hpp"

#include "fixtures/IntegrationFixture.h"

#include "ssp21/LogLevels.h"
#include "ssp21/ConsolePrettyPrinter.h"

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

	/*
    fix.ilog.log_everything();
    fix.ilog.add_backend(std::make_shared<ConsolePrettyPrinter>());

    fix.rlog.log_everything();
    fix.rlog.add_backend(std::make_shared<ConsolePrettyPrinter>());
	*/

    fix.responder->on_open();
    fix.initiator->on_open();

	REQUIRE_FALSE(fix.responder_upper.get_is_open());
	REQUIRE_FALSE(fix.initiator_upper.get_is_open());

    REQUIRE(fix.exe->run_many() > 0);

	REQUIRE(fix.responder_upper.get_is_open());
	REQUIRE(fix.initiator_upper.get_is_open());
}
