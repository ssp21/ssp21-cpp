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
// License TBD
//

#include "ssp21/gen/CertificateMode.h"

namespace ssp21 {

uint8_t CertificateModeSpec::to_type(CertificateMode arg)
{
    return static_cast<uint8_t>(arg);
}
CertificateMode CertificateModeSpec::from_type(uint8_t arg)
{
    switch(arg)
    {
        case(0x0):
            return CertificateMode::preshared_keys;
        case(0x1):
            return CertificateMode::m2m;
        default:
            return CertificateMode::undefined;
    }
}
const char* CertificateModeSpec::to_string(CertificateMode arg)
{
    switch(arg)
    {
        case(CertificateMode::preshared_keys):
            return "preshared_keys";
        case(CertificateMode::m2m):
            return "m2m";
        default:
            return "undefined";
    }
}

}
