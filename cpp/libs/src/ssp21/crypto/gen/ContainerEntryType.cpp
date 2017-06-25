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

#include "ssp21/crypto/gen/ContainerEntryType.h"

namespace ssp21 {

uint8_t ContainerEntryTypeSpec::to_type(ContainerEntryType arg)
{
    return static_cast<uint8_t>(arg);
}
ContainerEntryType ContainerEntryTypeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return ContainerEntryType::certificate_chain;
        case(0x1):
            return ContainerEntryType::x25519_public_key;
        case(0x2):
            return ContainerEntryType::x25519_private_key;
        case(0x3):
            return ContainerEntryType::ed25519_public_key;
        case(0x4):
            return ContainerEntryType::ed25519_private_key;
        default:
            return ContainerEntryType::undefined;
    }
}
const char* ContainerEntryTypeSpec::to_string(ContainerEntryType arg)
{
    switch(arg)
    {
        case(ContainerEntryType::certificate_chain):
            return "certificate_chain";
        case(ContainerEntryType::x25519_public_key):
            return "x25519_public_key";
        case(ContainerEntryType::x25519_private_key):
            return "x25519_private_key";
        case(ContainerEntryType::ed25519_public_key):
            return "ed25519_public_key";
        case(ContainerEntryType::ed25519_private_key):
            return "ed25519_private_key";
        default:
            return "undefined";
    }
}

}
