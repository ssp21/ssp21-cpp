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

#include "ssp21/msg/ReplyHandshakeBegin.h"

#include "ssp21/MessageParser.h"
#include "ssp21/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeBegin::ReplyHandshakeBegin() : 
  version(0)
{}

ReplyHandshakeBegin::ReplyHandshakeBegin(
  uint16_t version,
  const Seq8& ephemeral_public_key
) :
  version(version),
  ephemeral_public_key(ephemeral_public_key)
{}

ParseError ReplyHandshakeBegin::read(openpal::RSlice& input)
{
  return MessageParser::read_message<Function::reply_handshake_begin>(
    input,
    version,
    ephemeral_public_key,
    certificates
  );
}

FormatError ReplyHandshakeBegin::write(openpal::WSlice& output)
{
  return MessageFormatter::write_message<Function::reply_handshake_begin>(
    output,
    version,
    ephemeral_public_key,
    certificates
  );
}

FormatResult ReplyHandshakeBegin::write_msg(openpal::WSlice& output)
{
  auto write = [this](openpal::WSlice& output) { return this->write(output); };
  return FormatResult::write_any(write, output);
}

}
