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

#include "ser4cpp/util/Uncopyable.h"
#include <cstdint>

namespace ssp21 {

/**
    Enumerates possible cryptographic actions
*/
enum class CryptoAction : uint8_t {
    ///
    secure_equals = 0x0,
    ///
    gen_random = 0x1,
    ///
    hash_sha256 = 0x2,
    ///
    hmac_sha256 = 0x3,
    ///
    hkdf_sha256 = 0x4,
    ///
    gen_keypair_x25519 = 0x5,
    ///
    dh_x25519 = 0x6,
    ///
    gen_keypair_ed25519 = 0x7,
    ///
    sign_ed25519 = 0x8,
    ///
    verify_ed25519 = 0x9
};

struct CryptoActionSpec : private ser4cpp::StaticOnly {
    using enum_type_t = CryptoAction;

    static const char* to_string(CryptoAction arg);
};

}

#endif
