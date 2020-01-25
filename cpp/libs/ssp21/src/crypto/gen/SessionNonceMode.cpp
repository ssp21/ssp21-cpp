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

#include "ssp21/crypto/gen/SessionNonceMode.h"

namespace ssp21 {

uint8_t SessionNonceModeSpec::to_type(SessionNonceMode arg)
{
    return static_cast<uint8_t>(arg);
}
SessionNonceMode SessionNonceModeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return SessionNonceMode::increment_last_rx;
        case(0x1):
            return SessionNonceMode::greater_than_last_rx;
        default:
            return SessionNonceMode::undefined;
    }
}
const char* SessionNonceModeSpec::to_string(SessionNonceMode arg)
{
    switch(arg)
    {
        case(SessionNonceMode::increment_last_rx):
            return "increment_last_rx";
        case(SessionNonceMode::greater_than_last_rx):
            return "greater_than_last_rx";
        default:
            return "undefined";
    }
}

}
