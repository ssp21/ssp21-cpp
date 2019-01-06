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

#include "ssp21/crypto/gen/PublicKeyType.h"

namespace ssp21 {

uint8_t PublicKeyTypeSpec::to_type(PublicKeyType arg)
{
    return static_cast<uint8_t>(arg);
}
PublicKeyType PublicKeyTypeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return PublicKeyType::Ed25519;
        case(0x1):
            return PublicKeyType::X25519;
        default:
            return PublicKeyType::undefined;
    }
}
const char* PublicKeyTypeSpec::to_string(PublicKeyType arg)
{
    switch(arg)
    {
        case(PublicKeyType::Ed25519):
            return "Ed25519";
        case(PublicKeyType::X25519):
            return "X25519";
        default:
            return "undefined";
    }
}

}
