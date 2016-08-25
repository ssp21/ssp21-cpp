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
// License TBD
//

#include "ssp21/gen/DHMode.h"

namespace ssp21 {

uint8_t DHModeSpec::to_type(DHMode arg)
{
    return static_cast<uint8_t>(arg);
}
DHMode DHModeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return DHMode::x25519;
        default:
            return DHMode::undefined;
    }
}
const char* DHModeSpec::to_string(DHMode arg)
{
    switch(arg)
    {
        case(DHMode::x25519):
            return "x25519";
        default:
            return "undefined";
    }
}

}
