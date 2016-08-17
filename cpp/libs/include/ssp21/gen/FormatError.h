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

#ifndef SSP21_FORMATERROR_H
#define SSP21_FORMATERROR_H

#include "openpal/util/Uncopyable.h"
#include <cstdint>

namespace ssp21 {

/**
  The result of a message format operation
*/
enum class FormatError : uint8_t
{
  /// message was formatted successfully
  ok = 0x0,
  /// not enough output buffer space
  insufficient_space = 0x1,
  /// a message sequence length overflowed its serialized representation
  bad_sequence_length = 0x2
};

inline bool any(FormatError value)
{
  return value != FormatError::ok;
}

struct FormatErrorSpec : private openpal::StaticOnly
{
  typedef FormatError enum_type_t;

  static char const* to_string(FormatError arg);
};

}

#endif
