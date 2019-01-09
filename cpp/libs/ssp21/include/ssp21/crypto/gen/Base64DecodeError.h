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

#ifndef SSP21_BASE64DECODEERROR_H
#define SSP21_BASE64DECODEERROR_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    The result of a base64 decode operation
*/
enum class Base64DecodeError : uint8_t
{
    /// decode success
    ok = 0x0,
    /// The base64 input is not a multiple of four
    not_mult_four = 0x1,
    /// The input contains a non-base64 value
    not_base64 = 0x2,
    /// Trailing input after terminating characters
    bad_trailing_input = 0x3
};

inline bool any(Base64DecodeError value)
{
    return value != Base64DecodeError::ok;
}

struct Base64DecodeErrorSpec : private ser4cpp::StaticOnly
{
    typedef Base64DecodeError enum_type_t;

    static const char* to_string(Base64DecodeError arg);
};

}

#endif
