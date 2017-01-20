

#include "catch.hpp"

#include "fixtures/IntegrationFixture.h"

#include "ssp21/LogLevels.h"
#include "ssp21/ConsolePrettyPrinter.h"

#define SUITE(name) "IntegrationTestSuite - " name

using namespace ssp21;
using namespace openpal;

void open_and_test_handshake(IntegrationFixture& fix);
void enable_all_logging(IntegrationFixture& fix);

TEST_CASE(SUITE("fixture construction"))
{
    IntegrationFixture fix;
}

TEST_CASE(SUITE("completes handshake"))
{
    IntegrationFixture fix;
    open_and_test_handshake(fix);
}

TEST_CASE(SUITE("can transfer data from initiator to responder"))
{
    IntegrationFixture fix;
    open_and_test_handshake(fix);

    const auto size = 64;
    uint8_t payload[size] = { 0x00 };
    for (int i = 0; i < size; ++i) payload[i] = i % 255;
    const auto slice = seq32_t(payload, size);

    enable_all_logging(fix);

    fix.initiator->transmit(slice);
    REQUIRE(fix.exe->run_many() > 0);
    REQUIRE(fix.responder_upper.num_bytes_rx == 64);
}

void open_and_test_handshake(IntegrationFixture& fix)
{
    fix.responder->on_open();
    fix.initiator->on_open();

    REQUIRE_FALSE(fix.responder_upper.get_is_open());
    REQUIRE_FALSE(fix.initiator_upper.get_is_open());

    REQUIRE(fix.exe->run_many() > 0);

    REQUIRE(fix.responder_upper.get_is_open());
    REQUIRE(fix.initiator_upper.get_is_open());
}

void enable_all_logging(IntegrationFixture& fix)
{
    fix.ilog.log_everything();
    fix.ilog.add_backend(std::make_shared<ConsolePrettyPrinter>());

    fix.rlog.log_everything();
    fix.rlog.add_backend(std::make_shared<ConsolePrettyPrinter>());
}