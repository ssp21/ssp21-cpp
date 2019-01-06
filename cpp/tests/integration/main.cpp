
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "sodium/SodiumBackend.h"

int main(int argc, char*  argv[])
{
    // global setup...
    assert(ssp21::SodiumBackend::initialize());

    int result = Catch::Session().run(argc, argv);

    // global clean-up...

    return result;
}
