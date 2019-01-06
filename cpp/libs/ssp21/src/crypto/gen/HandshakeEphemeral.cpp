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

#include "ssp21/crypto/gen/HandshakeEphemeral.h"

namespace ssp21 {

uint8_t HandshakeEphemeralSpec::to_type(HandshakeEphemeral arg)
{
    return static_cast<uint8_t>(arg);
}
HandshakeEphemeral HandshakeEphemeralSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return HandshakeEphemeral::x25519;
        case(0x1):
            return HandshakeEphemeral::nonce;
        case(0x2):
            return HandshakeEphemeral::none;
        default:
            return HandshakeEphemeral::undefined;
    }
}
const char* HandshakeEphemeralSpec::to_string(HandshakeEphemeral arg)
{
    switch(arg)
    {
        case(HandshakeEphemeral::x25519):
            return "x25519";
        case(HandshakeEphemeral::nonce):
            return "nonce";
        case(HandshakeEphemeral::none):
            return "none";
        default:
            return "undefined";
    }
}

}
