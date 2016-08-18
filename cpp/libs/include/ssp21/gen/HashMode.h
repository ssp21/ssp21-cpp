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

#ifndef SSP21_HASHMODE_H
#define SSP21_HASHMODE_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
  Specifies which hash algorithm is used for the handshake hash and handshake authentication
*/
enum class HashMode : uint8_t
{
  /// Use SHA256 and HMAC-SHA256
  sha256 = 0x0,
  /// value not defined
  undefined = 0xFF
};

struct HashModeSpec : private openpal::StaticOnly
{
  typedef HashMode enum_type_t;

  static uint8_t to_type(HashMode arg);
  static HashMode from_type(uint8_t arg);
  static const char* to_string(HashMode arg);
};

}

#endif
