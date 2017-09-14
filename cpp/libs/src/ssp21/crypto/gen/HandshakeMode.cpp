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

#include "ssp21/crypto/gen/HandshakeMode.h"

namespace ssp21 {

uint8_t HandshakeModeSpec::to_type(HandshakeMode arg)
{
    return static_cast<uint8_t>(arg);
}
HandshakeMode HandshakeModeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return HandshakeMode::shared_secret;
        case(0x1):
            return HandshakeMode::preshared_public_keys;
        case(0x2):
            return HandshakeMode::industrial_certificates;
        default:
            return HandshakeMode::undefined;
    }
}
const char* HandshakeModeSpec::to_string(HandshakeMode arg)
{
    switch(arg)
    {
        case(HandshakeMode::shared_secret):
            return "shared_secret";
        case(HandshakeMode::preshared_public_keys):
            return "preshared_public_keys";
        case(HandshakeMode::industrial_certificates):
            return "industrial_certificates";
        default:
            return "undefined";
    }
}

}
