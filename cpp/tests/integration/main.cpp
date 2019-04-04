
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "sodium/CryptoBackend.h"

int main(int argc, char*  argv[])
{
    // global setup...
    assert(ssp21::sodium::CryptoBackend::initialize());

    int result = Catch::Session().run(argc, argv);

    // global clean-up...

    return result;
}
