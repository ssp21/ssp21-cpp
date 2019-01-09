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

#ifndef SSP21_PEMDECODEERROR_H
#define SSP21_PEMDECODEERROR_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"
#include "ssp21/util/ErrorCategory.h"

namespace ssp21 {

/**
    The result of a PEM decode operation
*/
enum class PEMDecodeError : uint8_t
{
    /// decode success
    ok = 0x0,
    /// End of file was reached before a complete decoding
    end_of_file = 0x1,
    /// a -----BEGIN <id>----- header was missing
    missing_begin_header = 0x2
};

inline bool any(PEMDecodeError value)
{
    return value != PEMDecodeError::ok;
}

struct PEMDecodeErrorSpec : private ser4cpp::StaticOnly
{
    typedef PEMDecodeError enum_type_t;

    static const char* name;

    static const char* to_string(PEMDecodeError arg);
};

typedef ErrorCategory<PEMDecodeErrorSpec> PEMDecodeErrorCategory;

inline std::error_code make_error_code(PEMDecodeError err)
{
    return std::error_code(static_cast<int>(err), PEMDecodeErrorCategory::get());
}

}

namespace std {

template <>
struct is_error_code_enum<ssp21::PEMDecodeError> : public true_type {};

}

#endif
