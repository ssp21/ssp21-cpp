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

#ifndef SSP21_HANDSHAKEMODE_H
#define SSP21_HANDSHAKEMODE_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    specifies how key derivation occurs
*/
enum class HandshakeMode : uint8_t
{
    /// both parties have a shared secret
    shared_secret = 0x0,
    /// both parties know each others public DH keys
    preshared_public_keys = 0x1,
    /// Industrial certificate format
    industrial_certificates = 0x2,
    /// Single-use shared secrets
    quantum_key_distribution = 0x3,
    /// value not defined
    undefined = 0xFF
};

struct HandshakeModeSpec : private ser4cpp::StaticOnly
{
    typedef HandshakeMode enum_type_t;

    static uint8_t to_type(HandshakeMode arg);
    static HandshakeMode from_type(uint8_t arg);
    static const char* to_string(HandshakeMode arg);
};

}

#endif
