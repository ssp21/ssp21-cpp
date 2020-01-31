
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "sodium/Backend.h"

int main(int argc, char* argv[])
{
    // global setup...
    ssp21::sodium::initialize();

    int result = Catch::Session().run(argc, argv);

    // global clean-up...

    return result;
}
