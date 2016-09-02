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

#ifndef SSP21_CRYPTOERROR_H
#define SSP21_CRYPTOERROR_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    The result of a message parse operation
*/
enum class CryptoError : uint8_t
{
    /// attempted operation with bad key type
    bad_key_type = 0,
    /// X25519 DH operation failed. Bad public key?
    dh_x25519_fail = 1,
    /// a procedure cannot be executed b/c of an undefined algorithm
    undefined_algorithm = 2,
    /// A calculated MAC did not match the specified MAC
    mac_auth_fail = 3,
    /// A buffer had an insufficient or incorrect size
    bad_buffer_size = 4
};

struct CryptoErrorSpec : private openpal::StaticOnly
{
    typedef CryptoError enum_type_t;

    static const char* to_string(CryptoError arg);
};

}

#endif
