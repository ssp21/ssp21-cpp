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

#ifndef SSP21_PUBLICKEYTYPE_H
#define SSP21_PUBLICKEYTYPE_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    Defines the type of the public key embedded in a certificate body
*/
enum class PublicKeyType : uint8_t
{
    /// The key is an Ed25519 DSA public key
    Ed25519 = 0x0,
    /// The key is an x25519 DH key
    X25519 = 0x1,
    /// value not defined
    undefined = 0xFF
};

struct PublicKeyTypeSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = PublicKeyType;

    static uint8_t to_type(PublicKeyType arg);
    static PublicKeyType from_type(uint8_t arg);
    static const char* to_string(PublicKeyType arg);
};

}

#endif
