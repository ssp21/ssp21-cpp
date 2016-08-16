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

#include "ssp21/gen/HashMode.h"

namespace ssp21 {

uint8_t HashModeSpec::to_type(HashMode arg)
{
  return static_cast<uint8_t>(arg);
}
HashMode HashModeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return HashMode::sha256;
    default:
      return HashMode::undefined;
  }
}
char const* HashModeSpec::to_string(HashMode arg)
{
  switch(arg)
  {
    case(HashMode::sha256):
      return "sha256";
    default:
      return "undefined";
  }
}

}
