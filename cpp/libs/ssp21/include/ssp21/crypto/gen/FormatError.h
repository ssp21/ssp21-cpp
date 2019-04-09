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

#ifndef SSP21_FORMATERROR_H
#define SSP21_FORMATERROR_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"
#include "ssp21/util/ErrorCategory.h"

namespace ssp21 {

/**
    The result of a message format operation
*/
enum class FormatError : uint8_t
{
    /// message was formatted successfully
    ok = 0x0,
    /// not enough output buffer space
    insufficient_space = 0x1
};

inline bool any(FormatError value)
{
    return value != FormatError::ok;
}

struct FormatErrorSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = FormatError;

    static const char* name;

    static const char* to_string(FormatError arg);
};

using FormatErrorCategory = ErrorCategory<FormatErrorSpec>;

inline std::error_code make_error_code(FormatError err)
{
    return std::error_code(static_cast<int>(err), FormatErrorCategory::get());
}

}

namespace std {

template <>
struct is_error_code_enum<ssp21::FormatError> : public true_type {};

}

#endif
