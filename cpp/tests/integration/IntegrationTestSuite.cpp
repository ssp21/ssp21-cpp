

#include "catch.hpp"

#include "fixtures/IntegrationFixture.h"

#include "ssp21/stack/LogLevels.h"
#include "ssp21/util/ConsolePrettyPrinter.h"

#define SUITE(name) "IntegrationTestSuite - " name

using namespace ssp21;
using namespace openpal;

void open_and_test_handshake(IntegrationFixture& fix);
void test_bidirectional_data_transfer(IntegrationFixture& fix, const seq32_t& data);
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

TEST_CASE(SUITE("can transfer data bidirectionally multiple times"))
{
    IntegrationFixture fix;
    open_and_test_handshake(fix);

    const auto num_bytes_tx = 64;
    uint8_t payload[num_bytes_tx] = { 0x00 };
    for (int i = 0; i < num_bytes_tx; ++i) payload[i] = i % 256;
    const auto slice = seq32_t(payload, num_bytes_tx);

    for (int i = 0; i < 5; ++i)
    {
        test_bidirectional_data_transfer(fix, slice);
    }
}

void open_and_test_handshake(IntegrationFixture& fix)
{
    fix.responder->get_upper().on_lower_open();
    fix.initiator->get_upper().on_lower_open();

    REQUIRE_FALSE(fix.responder_upper.is_open());
    REQUIRE_FALSE(fix.initiator_upper.is_open());

    REQUIRE(fix.exe->run_many() > 0);

    REQUIRE(fix.responder_upper.is_open());
    REQUIRE(fix.initiator_upper.is_open());
}

void test_bidirectional_data_transfer(IntegrationFixture& fix, const seq32_t& data)
{
    REQUIRE(fix.responder_validator->is_empty());
    REQUIRE(fix.initiator_validator->is_empty());

    fix.responder_validator->expect(data);
    fix.initiator_validator->expect(data);

    REQUIRE(fix.initiator->get_lower().start_tx_from_upper(data));
    REQUIRE(fix.responder->get_lower().start_tx_from_upper(data));

    REQUIRE(fix.exe->run_many() > 0);

    REQUIRE(fix.responder_validator->is_empty());
    REQUIRE(fix.initiator_validator->is_empty());
}

void enable_all_logging(IntegrationFixture& fix)
{
    fix.ilog.log_everything();
    fix.ilog.add_backend(std::make_shared<ConsolePrettyPrinter>());

    fix.rlog.log_everything();
    fix.rlog.add_backend(std::make_shared<ConsolePrettyPrinter>());
}