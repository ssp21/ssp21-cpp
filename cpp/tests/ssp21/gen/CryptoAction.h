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

#ifndef SSP21_CRYPTOACTION_H
#define SSP21_CRYPTOACTION_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    Enumerates possible cryptographic actions
*/
enum class CryptoAction : uint8_t
{
    /// 
    secure_equals = 0x0,
    /// 
    hash_sha256 = 0x1,
    /// 
    hmac_sha256 = 0x2,
    /// 
    hkdf_sha256 = 0x3,
    /// 
    gen_keypair_x25519 = 0x4,
    /// 
    dh_x25519 = 0x5
};

struct CryptoActionSpec : private openpal::StaticOnly
{
    typedef CryptoAction enum_type_t;

    static const char* to_string(CryptoAction arg);
};

}

#endif
