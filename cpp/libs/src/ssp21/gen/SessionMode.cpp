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

#include "ssp21/gen/SessionMode.h"

namespace ssp21 {

uint8_t SessionModeSpec::to_type(SessionMode arg)
{
    return static_cast<uint8_t>(arg);
}
SessionMode SessionModeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return SessionMode::hmac_sha256_16;
        default:
            return SessionMode::undefined;
    }
}
const char* SessionModeSpec::to_string(SessionMode arg)
{
    switch(arg)
    {
        case(SessionMode::hmac_sha256_16):
            return "hmac_sha256_16";
        default:
            return "undefined";
    }
}

}
