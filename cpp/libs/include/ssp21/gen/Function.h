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

#ifndef SSP21_FUNCTION_H
#define SSP21_FUNCTION_H

#include "openpal/util/Uncopyable.h"
#include <cstdint>

namespace ssp21 {

/**
  Crypto layer function code
*/
enum class Function : uint8_t
{
  /// Master sends this initiate a handshake
  request_handshake_begin = 0x0,
  /// Outstation replies to the handshake begin from the master
  reply_handshake_begin = 0x1,
  /// Master authenticates to the outstation
  request_handshake_auth = 0x2,
  /// Outstation authenticates to the master
  reply_handshake_auth = 0x3,
  /// Outstation returns an error code to the master
  reply_handshake_error = 0x4,
  /// Either party transmits an unconfirmed session message
  unconfirmed_session_data = 0x5,
  /// Unknown function code. Used internally in ssp21 to indicate the code didn't match anything known
  undefined = 0xFF
};

struct FunctionSpec : private openpal::StaticOnly
{
  typedef Function enum_type_t;

  static uint8_t to_type(Function arg);
  static Function from_type(uint8_t arg);
  static const char* to_string(Function arg);
};

}

#endif
