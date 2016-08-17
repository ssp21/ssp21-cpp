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

#ifndef SSP21_DHMODE_H
#define SSP21_DHMODE_H

#include "openpal/util/Uncopyable.h"
#include <cstdint>

namespace ssp21 {

/**
  Specifies which Diffie Hellman function is used
*/
enum class DHMode : uint8_t
{
  /// Use the x25519 algorithm
  x25519 = 0x0,
  /// undefined mode
  undefined = 0xFF
};

struct DHModeSpec : private openpal::StaticOnly
{
  typedef DHMode enum_type_t;

  static uint8_t to_type(DHMode arg);
  static DHMode from_type(uint8_t arg);
  static const char* to_string(DHMode arg);
};

}

#endif
