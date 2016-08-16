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

#include "ssp21/gen/Function.h"

namespace ssp21 {

uint8_t FunctionSpec::to_type(Function arg)
{
  return static_cast<uint8_t>(arg);
}
Function FunctionSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return Function::request_handshake_begin;
    case(0x1):
      return Function::reply_handshake_begin;
    case(0x2):
      return Function::request_handshake_auth;
    case(0x3):
      return Function::reply_handshake_auth;
    case(0x4):
      return Function::reply_handshake_error;
    case(0x5):
      return Function::unconfirmed_session_data;
    default:
      return Function::undefined;
  }
}
char const* FunctionSpec::to_string(Function arg)
{
  switch(arg)
  {
    case(Function::request_handshake_begin):
      return "request_handshake_begin";
    case(Function::reply_handshake_begin):
      return "reply_handshake_begin";
    case(Function::request_handshake_auth):
      return "request_handshake_auth";
    case(Function::reply_handshake_auth):
      return "reply_handshake_auth";
    case(Function::reply_handshake_error):
      return "reply_handshake_error";
    case(Function::unconfirmed_session_data):
      return "unconfirmed_session_data";
    default:
      return "undefined";
  }
}

}
