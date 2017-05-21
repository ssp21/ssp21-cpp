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

#include "ssp21/crypto/gen/FileEntryType.h"

namespace ssp21 {

uint8_t FileEntryTypeSpec::to_type(FileEntryType arg)
{
    return static_cast<uint8_t>(arg);
}
FileEntryType FileEntryTypeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return FileEntryType::certificate;
        case(0x1):
            return FileEntryType::x25519_public_key;
        case(0x2):
            return FileEntryType::x25519_private_key;
        case(0x3):
            return FileEntryType::ed25519_public_key;
        case(0x4):
            return FileEntryType::ed25519_private_key;
        default:
            return FileEntryType::undefined;
    }
}
const char* FileEntryTypeSpec::to_string(FileEntryType arg)
{
    switch(arg)
    {
        case(FileEntryType::certificate):
            return "certificate";
        case(FileEntryType::x25519_public_key):
            return "x25519_public_key";
        case(FileEntryType::x25519_private_key):
            return "x25519_private_key";
        case(FileEntryType::ed25519_public_key):
            return "ed25519_public_key";
        case(FileEntryType::ed25519_private_key):
            return "ed25519_private_key";
        default:
            return "undefined";
    }
}

}
