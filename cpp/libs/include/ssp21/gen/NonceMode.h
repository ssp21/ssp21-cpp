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

#ifndef SSP21_NONCEMODE_H
#define SSP21_NONCEMODE_H

#include <cstdint>

namespace ssp21 {

/**
  Determines how nonces are verified during the session
*/
enum class NonceMode : uint8_t
{
  /// new nonce must strictly be equal to last nonce plus one
  increment_last_rx = 0x0,
  /// new nonce must be greater than last nonce
  greater_than_last_rx = 0x1,
  /// Undefined mode
  undefined = 0xFF
};

uint8_t noncemode_to_type(NonceMode arg);
NonceMode noncemode_from_type(uint8_t arg);
char const* noncemode_to_string(NonceMode arg);

}

#endif
