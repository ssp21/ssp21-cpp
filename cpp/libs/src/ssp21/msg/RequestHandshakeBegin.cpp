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

#include "ssp21/msg/RequestHandshakeBegin.h"

namespace ssp21 {

RequestHandshakeBegin::RequestHandshakeBegin() : 
  function(Function::undefined),
  version(0),
  nonce_mode(NonceMode::undefined),
  dh_mode(DHMode::undefined),
  hash_mode(HashMode::undefined),
  session_mode(SessionMode::undefined),
  certificate_mode(CertificateMode::undefined)
{
}

}
