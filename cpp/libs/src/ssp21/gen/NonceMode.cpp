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

#include "ssp21/gen/NonceMode.h"

namespace ssp21 {

uint8_t NonceModeSpec::to_type(NonceMode arg)
{
  return static_cast<uint8_t>(arg);
}
NonceMode NonceModeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return NonceMode::increment_last_rx;
    case(0x1):
      return NonceMode::greater_than_last_rx;
    default:
      return NonceMode::undefined;
  }
}
char const* NonceModeSpec::to_string(NonceMode arg)
{
  switch(arg)
  {
    case(NonceMode::increment_last_rx):
      return "increment_last_rx";
    case(NonceMode::greater_than_last_rx):
      return "greater_than_last_rx";
    default:
      return "undefined";
  }
}

}
