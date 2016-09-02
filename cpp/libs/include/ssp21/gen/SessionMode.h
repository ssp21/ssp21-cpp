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

#ifndef SSP21_SESSIONMODE_H
#define SSP21_SESSIONMODE_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    Specifies the complete set of algorithms used to secure the session
*/
enum class SessionMode : uint8_t
{
    /// HMAC-SHA256 truncated to 16 bytes
    hmac_sha256_16 = 0x0,
    /// value not defined
    undefined = 0xFF
};

struct SessionModeSpec : private openpal::StaticOnly
{
    typedef SessionMode enum_type_t;

    static uint8_t to_type(SessionMode arg);
    static SessionMode from_type(uint8_t arg);
    static const char* to_string(SessionMode arg);
};

}

#endif
