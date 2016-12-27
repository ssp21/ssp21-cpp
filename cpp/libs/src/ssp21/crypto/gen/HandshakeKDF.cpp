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

#include "ssp21/crypto/gen/HandshakeKDF.h"

namespace ssp21 {

uint8_t HandshakeKDFSpec::to_type(HandshakeKDF arg)
{
    return static_cast<uint8_t>(arg);
}
HandshakeKDF HandshakeKDFSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return HandshakeKDF::HKDF_SHA256;
        default:
            return HandshakeKDF::undefined;
    }
}
const char* HandshakeKDFSpec::to_string(HandshakeKDF arg)
{
    switch(arg)
    {
        case(HandshakeKDF::HKDF_SHA256):
            return "HKDF_SHA256";
        default:
            return "undefined";
    }
}

}
