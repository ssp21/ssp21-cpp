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

#include "ssp21/gen/ParseError.h"

namespace ssp21 {

const char* ParseErrorSpec::to_string(ParseError arg)
{
  switch(arg)
  {
    case(ParseError::ok):
      return "ok";
    case(ParseError::insufficient_bytes):
      return "insufficient_bytes";
    case(ParseError::undefined_enum):
      return "undefined_enum";
    case(ParseError::unexpected_function):
      return "unexpected_function";
    case(ParseError::too_many_bytes):
      return "too_many_bytes";
    case(ParseError::impl_capacity_limit):
      return "impl_capacity_limit";
    default:
      return "undefined";
  }
}

}
