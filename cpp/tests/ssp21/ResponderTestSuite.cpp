

#include "catch.hpp"

#include "ssp21/Responder.h"

#include "openpal/container/StaticBuffer.h"

#include "testlib/BufferHelpers.h"

#include "mocks/MockLogger.h"

#define SUITE(name) "ResponderTestSuite - " name

using namespace ssp21;
using namespace openpal;

TEST_CASE(SUITE("can be constructed"))
{		
	MockLogger log("responder");
	
	Responder resp(log.root.logger);	
}