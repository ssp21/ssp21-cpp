
#include "sodiumbackend/Initialize.h"

#include "ssp21/crypto/Crypto.h"

#include "SodiumBackend.h"

#include <sodium.h>

namespace ssp21
{
namespace sodium
{

bool initialize()
{
    if (sodium_init() == -1)
    {
        return false;
    }

    Crypto::inititalize(SodiumBackend::Instance());

    return true;
}


}
}


