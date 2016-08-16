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

#include "ssp21/gen/CryptoError.h"

namespace ssp21 {

char const* crypto_error_to_string(CryptoError arg)
{
  switch(arg)
  {
    case(CryptoError::bad_key_type):
      return "bad_key_type";
    case(CryptoError::dh_x25519_fail):
      return "dh_x25519_fail";
    default:
      return "undefined";
  }
}

}
