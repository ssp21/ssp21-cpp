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

#ifndef SSP21_CONTAINERENTRYTYPE_H
#define SSP21_CONTAINERENTRYTYPE_H

#include <cstdint>
#include "ser4cpp/util/Uncopyable.h"

namespace ssp21 {

/**
    enumerates the possible entry types in a container file
*/
enum class ContainerEntryType : uint8_t
{
    /// 256-bit shared secret
    shared_secret = 0x0,
    /// industrial certificate chain
    certificate_chain = 0x1,
    /// X25519 private key
    x25519_public_key = 0x2,
    /// X25519 private key
    x25519_private_key = 0x3,
    /// Ed25519 private key
    ed25519_public_key = 0x4,
    /// Ed25519 private key
    ed25519_private_key = 0x5,
    /// value not defined
    undefined = 0xFF
};

struct ContainerEntryTypeSpec : private ser4cpp::StaticOnly
{
    using enum_type_t = ContainerEntryType;

    static uint8_t to_type(ContainerEntryType arg);
    static ContainerEntryType from_type(uint8_t arg);
    static const char* to_string(ContainerEntryType arg);
};

}

#endif
