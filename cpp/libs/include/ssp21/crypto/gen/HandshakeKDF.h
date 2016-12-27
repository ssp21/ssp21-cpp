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

#ifndef SSP21_HANDSHAKEKDF_H
#define SSP21_HANDSHAKEKDF_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    Specifies the Key Derivation Function (KDF) used during the handshake
*/
enum class HandshakeKDF : uint8_t
{
    /// Use HKDF with HMAC-SHA256
    HKDF_SHA256 = 0x0,
    /// value not defined
    undefined = 0xFF
};

struct HandshakeKDFSpec : private openpal::StaticOnly
{
    typedef HandshakeKDF enum_type_t;

    static uint8_t to_type(HandshakeKDF arg);
    static HandshakeKDF from_type(uint8_t arg);
    static const char* to_string(HandshakeKDF arg);
};

}

#endif
