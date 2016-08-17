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

#include "ssp21/msg/ReplyHandshakeError.h"

#include "ssp21/MessageParser.h"
#include "ssp21/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeError::ReplyHandshakeError() : 
  handshake_error(HandshakeError::undefined)
{
}

ParseError ReplyHandshakeError::read(openpal::RSlice& input)
{
  return MessageParser::read_message<Function::reply_handshake_error>(
    input,
    handshake_error
  );
}

FormatError ReplyHandshakeError::write(openpal::WSlice& dest)
{
  return MessageFormatter::write_message<Function::reply_handshake_error>(
    dest,
    handshake_error
  );
}

}
