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

#ifndef SSP21_SESSIONNONCEMODE_H
#define SSP21_SESSIONNONCEMODE_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    Determines how nonces are verified during the session
*/
enum class SessionNonceMode : uint8_t
{
    /// new nonce must strictly be equal to last nonce plus one
    increment_last_rx = 0x0,
    /// new nonce must be greater than last nonce
    greater_than_last_rx = 0x1,
    /// value not defined
    undefined = 0xFF
};

struct SessionNonceModeSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = SessionNonceMode;

    static uint8_t to_type(SessionNonceMode arg);
    static SessionNonceMode from_type(uint8_t arg);
    static const char* to_string(SessionNonceMode arg);
};

}

#endif
