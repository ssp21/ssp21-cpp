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

#include "ssp21/gen/ParseResult.h"

namespace ssp21 {

char const* ParseResultSpec::to_string(ParseResult arg)
{
  switch(arg)
  {
    case(ParseResult::ok):
      return "ok";
    case(ParseResult::insufficient_bytes):
      return "insufficient_bytes";
    case(ParseResult::undefined_enum):
      return "undefined_enum";
    case(ParseResult::unexpected_function):
      return "unexpected_function";
    case(ParseResult::too_many_bytes):
      return "too_many_bytes";
    case(ParseResult::impl_capacity_limit):
      return "impl_capacity_limit";
    default:
      return "undefined";
  }
}

}
