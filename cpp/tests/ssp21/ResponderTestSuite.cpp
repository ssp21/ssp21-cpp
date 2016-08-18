

#include "catch.hpp"

#include "ssp21/Responder.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/MockLogHandler.h"
#include "testlib/BufferHelpers.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("can be constructed"))
{		
	MockLogHandler handler;
	LogRoot root(&handler, "responder", ~0);
	//handler.write_to_stdio();

	Responder resp(root.logger);	

	resp.on_rx_data(RSlice::empty_slice());
}