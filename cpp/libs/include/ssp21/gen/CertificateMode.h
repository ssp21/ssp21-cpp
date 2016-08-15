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

#ifndef SSP21_CERTIFICATEMODE_H
#define SSP21_CERTIFICATEMODE_H

#include <cstdint>

namespace ssp21 {

/**
  specifies what type of certificate is exchanged by both parties to authenticate each other
*/
enum class CertificateMode : uint8_t
{
  /// both parties know each others public DH keys
  preshared_keys = 0x0,
  /// machine-to-machine certificate format
  m2m = 0x1,
  /// undefined mode
  undefined = 0xFF
};

uint8_t certificate_mode_to_type(CertificateMode arg);
CertificateMode certificate_mode_from_type(uint8_t arg);
char const* certificate_mode_to_string(CertificateMode arg);

}

#endif
