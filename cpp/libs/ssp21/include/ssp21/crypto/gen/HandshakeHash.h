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

#ifndef SSP21_HANDSHAKEHASH_H
#define SSP21_HANDSHAKEHASH_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    Specifies which hash algorithm is used for the handshake hash to calculate the chaining key
*/
enum class HandshakeHash : uint8_t
{
    /// Use SHA256
    sha256 = 0x0,
    /// value not defined
    undefined = 0xFF
};

struct HandshakeHashSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = HandshakeHash;

    static uint8_t to_type(HandshakeHash arg);
    static HandshakeHash from_type(uint8_t arg);
    static const char* to_string(HandshakeHash arg);
};

}

#endif
