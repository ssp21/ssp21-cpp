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

#ifndef SSP21_FILEENTRYTYPE_H
#define SSP21_FILEENTRYTYPE_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    enumerates the possible entry types in a certificate file
*/
enum class FileEntryType : uint8_t
{
    /// Industrial certificate format (ICF)
    certificate = 0x0,
    /// X25519 private key
    x25519_private_key = 0x1,
    /// Ed25519 private key
    ed25519_private_key = 0x2,
    /// value not defined
    undefined = 0xFF
};

struct FileEntryTypeSpec : private openpal::StaticOnly
{
    typedef FileEntryType enum_type_t;

    static uint8_t to_type(FileEntryType arg);
    static FileEntryType from_type(uint8_t arg);
    static const char* to_string(FileEntryType arg);
};

}

#endif
