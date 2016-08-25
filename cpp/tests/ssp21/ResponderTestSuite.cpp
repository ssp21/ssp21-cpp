

#include "catch.hpp"

#include "ssp21/crypto/Responder.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/BufferHelpers.h"
#include "testlib/MockExecutor.h"

#include "mocks/MockLogHandler.h"
#include "mocks/MockLowerLayer.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("can be constructed"))
{
    MockLogHandler log("responder");

	MockExecutor exe;
    MockLowerLayer lower;

    Responder responder(Responder::Config(), exe, log.root.logger, lower);
    responder.on_open();

    // request handshake begin with header only
    lower.enqueue_message(Addresses(5, 5), "00");
    responder.on_rx_ready();

    // ReplyHandshakeError w/ error = bad message format
    REQUIRE(lower.pop_tx_message() == "04 00");
}