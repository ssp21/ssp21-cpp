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

#ifndef SSP21_CERTIFICATEMODE_H
#define SSP21_CERTIFICATEMODE_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    specifies what type of certificate is exchanged by both parties to authenticate each other
*/
enum class CertificateMode : uint8_t
{
    /// both parties know each others public DH keys
    preshared_keys = 0x0,
    /// Industrial certificate format
    icf = 0x1,
    /// value not defined
    undefined = 0xFF
};

struct CertificateModeSpec : private openpal::StaticOnly
{
    typedef CertificateMode enum_type_t;

    static uint8_t to_type(CertificateMode arg);
    static CertificateMode from_type(uint8_t arg);
    static const char* to_string(CertificateMode arg);
};

}

#endif
