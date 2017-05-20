
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "ssp21/crypto/Crypto.h"

int main(int argc, char*  argv[])
{
    // global setup...
    if (!ssp21::Crypto::initialize())
    {
        std::cerr << "Error initializing crypto backend" << std::endl;
        return -1;
    }

    int result = Catch::Session().run(argc, argv);

    // global clean-up...

    return result;
}
