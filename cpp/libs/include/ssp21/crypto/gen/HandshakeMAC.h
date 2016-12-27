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

#ifndef SSP21_HANDSHAKEMAC_H
#define SSP21_HANDSHAKEMAC_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    Specifies the Message Authentication Code (MAC) algorithm used to authenticate the handshake
*/
enum class HandshakeMAC : uint8_t
{
    /// Use HMAC-SHA256
    HMAC_SHA256 = 0x0,
    /// value not defined
    undefined = 0xFF
};

struct HandshakeMACSpec : private openpal::StaticOnly
{
    typedef HandshakeMAC enum_type_t;

    static uint8_t to_type(HandshakeMAC arg);
    static HandshakeMAC from_type(uint8_t arg);
    static const char* to_string(HandshakeMAC arg);
};

}

#endif
