
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "mocks/MockCryptoBackend.h"
#include "ssp21/crypto/Crypto.h"

int main(int argc, char*  argv[])
{
    // global setup...
    if(!ssp21::MockCryptoBackend::initialize())
    {
        std::cerr << "unable to initialize mock cyrpto backend" << std::endl;
    }

    return Catch::Session().run(argc, argv);
}
