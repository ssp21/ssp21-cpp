//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Licensed under the terms of the BSDv3 license
//

#include "ssp21/crypto/gen/SessionCryptoMode.h"

namespace ssp21 {

uint8_t SessionCryptoModeSpec::to_type(SessionCryptoMode arg)
{
    return static_cast<uint8_t>(arg);
}
SessionCryptoMode SessionCryptoModeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return SessionCryptoMode::hmac_sha256_16;
        default:
            return SessionCryptoMode::undefined;
    }
}
const char* SessionCryptoModeSpec::to_string(SessionCryptoMode arg)
{
    switch(arg)
    {
        case(SessionCryptoMode::hmac_sha256_16):
            return "hmac_sha256_16";
        default:
            return "undefined";
    }
}

}
