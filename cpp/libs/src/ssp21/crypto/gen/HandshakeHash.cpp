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

#include "ssp21/crypto/gen/HandshakeHash.h"

namespace ssp21 {

uint8_t HandshakeHashSpec::to_type(HandshakeHash arg)
{
    return static_cast<uint8_t>(arg);
}
HandshakeHash HandshakeHashSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return HandshakeHash::sha256;
        default:
            return HandshakeHash::undefined;
    }
}
const char* HandshakeHashSpec::to_string(HandshakeHash arg)
{
    switch(arg)
    {
        case(HandshakeHash::sha256):
            return "sha256";
        default:
            return "undefined";
    }
}

}
