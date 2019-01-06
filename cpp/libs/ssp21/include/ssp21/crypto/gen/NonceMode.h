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

#ifndef SSP21_NONCEMODE_H
#define SSP21_NONCEMODE_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    Determines how nonces are verified during the session
*/
enum class NonceMode : uint8_t
{
    /// new nonce must strictly be equal to last nonce plus one
    increment_last_rx = 0x0,
    /// new nonce must be greater than last nonce
    greater_than_last_rx = 0x1,
    /// value not defined
    undefined = 0xFF
};

struct NonceModeSpec : private openpal::StaticOnly
{
    typedef NonceMode enum_type_t;

    static uint8_t to_type(NonceMode arg);
    static NonceMode from_type(uint8_t arg);
    static const char* to_string(NonceMode arg);
};

}

#endif
