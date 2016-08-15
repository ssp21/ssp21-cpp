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

namespace ssp21 {

/**
  Specifies which hash algorithm is used for the handshake hash and handshake authentication
*/
enum class HashMode : uint8_t
{
  /// Use SHA256 and HMAC-SHA256
  sha256 = 0x0,
  /// undefined mode
  undefined = 0xFF
};

uint8_t hash_mode_to_type(HashMode arg);
HashMode hash_mode_from_type(uint8_t arg);
char const* hash_mode_to_string(HashMode arg);

}

#endif
