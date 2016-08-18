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

#include "ssp21/msg/ReplyHandshakeAuth.h"

#include "ssp21/MessageParser.h"
#include "ssp21/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeAuth::ReplyHandshakeAuth()
{}

ReplyHandshakeAuth::ReplyHandshakeAuth(
  const Seq8& mac
) :
  mac(mac)
{}

ParseError ReplyHandshakeAuth::read_msg(openpal::RSlice& input)
{
  return MessageParser::read_message<Function::request_handshake_auth>(
    input,
    mac
  );
}

FormatError ReplyHandshakeAuth::write(openpal::WSlice& output)
{
  return MessageFormatter::write_message<Function::request_handshake_auth>(
    output,
    mac
  );
}

FormatResult ReplyHandshakeAuth::write_msg(openpal::WSlice& output)
{
  auto write = [this](openpal::WSlice& output) { return this->write(output); };
  return FormatResult::write_any(write, output);
}

}
