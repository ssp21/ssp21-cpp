

#include "catch.hpp"

#include "ssp21/crypto/Responder.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/BufferHelpers.h"

#include "mocks/MockLogger.h"
#include "mocks/MockLowerLayer.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("can be constructed"))
{		
	MockLogger log("responder");
	
	MockLowerLayer lower;

	Responder responder(Responder::Config(), log.root.logger, lower);	
	responder.on_open();

	// request handshake begin with header only
	lower.enqueue_message(Addresses(5, 5), "00");	
	responder.on_rx_ready();

	// ReplyHandshakeError w/ error = bad message format
	REQUIRE(lower.pop_tx_message() == "04 00");
}