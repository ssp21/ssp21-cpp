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

#include "ssp21/crypto/gen/HandshakeMAC.h"

namespace ssp21 {

uint8_t HandshakeMACSpec::to_type(HandshakeMAC arg)
{
    return static_cast<uint8_t>(arg);
}
HandshakeMAC HandshakeMACSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return HandshakeMAC::hmac_sha256;
        default:
            return HandshakeMAC::undefined;
    }
}
const char* HandshakeMACSpec::to_string(HandshakeMAC arg)
{
    switch(arg)
    {
        case(HandshakeMAC::hmac_sha256):
            return "hmac_sha256";
        default:
            return "undefined";
    }
}

}
