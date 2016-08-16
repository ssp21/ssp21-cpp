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

#ifndef SSP21_PARSERESULT_H
#define SSP21_PARSERESULT_H

#include "openpal/util/Uncopyable.h"
#include <cstdint>

namespace ssp21 {

/**
  The result of a message parse operation
*/
enum class ParseResult : uint8_t
{
  /// message was parsed successfully
  ok = 0x0,
  /// parser ran out of bytes before completion
  insufficient_bytes = 0x1,
  /// parser completed with bytes still remaining
  too_many_bytes = 0x2
};

struct ParseResultSpec : private openpal::StaticOnly
{
  typedef ParseResult enum_type_t;

  static char const* to_string(ParseResult arg);
};

}

#endif
