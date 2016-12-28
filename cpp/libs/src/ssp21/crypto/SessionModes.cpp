
#include "ssp21/crypto/SessionModes.h"

#include "ssp21/crypto/Crypto.h"

namespace ssp21
{

    TruncatedMacSessionMode SessionModes::hmac_sha_256_trunc16(Crypto::hmac_sha256, consts::crypto::trunc16, BufferType::mac_trunc16);

}


