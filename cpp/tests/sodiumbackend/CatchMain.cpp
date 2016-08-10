
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "sodiumbackend\Initialize.h"

int main(int argc, char* const argv[])
{
	// global setup...

	assert(ssp21::sodium::Initialize());

	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	return result;
}