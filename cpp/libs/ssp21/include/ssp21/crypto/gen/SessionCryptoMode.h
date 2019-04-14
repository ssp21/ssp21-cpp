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

#ifndef SSP21_SESSIONCRYPTOMODE_H
#define SSP21_SESSIONCRYPTOMODE_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    Specifies the complete set of algorithms used to secure the session
*/
enum class SessionCryptoMode : uint8_t
{
    /// HMAC-SHA256 truncated to 16 bytes
    hmac_sha256_16 = 0x0,
    /// value not defined
    undefined = 0xFF
};

struct SessionCryptoModeSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = SessionCryptoMode;

    static uint8_t to_type(SessionCryptoMode arg);
    static SessionCryptoMode from_type(uint8_t arg);
    static const char* to_string(SessionCryptoMode arg);
};

}

#endif
