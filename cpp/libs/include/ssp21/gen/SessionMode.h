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

#ifndef SSP21_SESSIONMODE_H
#define SSP21_SESSIONMODE_H

#include <cstdint>

namespace ssp21 {

/**
  Specifies the complete set of algorithms used to secure the session
*/
enum class SessionMode : uint8_t
{
  /// HMAC-SHA256 truncated to 16 bytes
  hmac_sha256_16 = 0x0,
  /// undefined mode
  undefined = 0xFF
};

uint8_t session_mode_to_type(SessionMode arg);
SessionMode session_mode_from_type(uint8_t arg);
char const* session_mode_to_string(SessionMode arg);

}

#endif
