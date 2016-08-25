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

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeError::ReplyHandshakeError() : 
  handshake_error(HandshakeError::undefined)
{}

ReplyHandshakeError::ReplyHandshakeError(
  HandshakeError handshake_error
) :
  handshake_error(handshake_error)
{}

ParseError ReplyHandshakeError::read_msg(const openpal::RSlice& input)
{
  return MessageParser::read_message<Function::reply_handshake_error>(
    input,
    handshake_error
  );
}

FormatError ReplyHandshakeError::write(openpal::WSlice& output) const
{
  return MessageFormatter::write_message<Function::reply_handshake_error>(
    output,
    handshake_error
  );
}

FormatResult ReplyHandshakeError::write_msg(openpal::WSlice& output) const
{
  auto write = [this](openpal::WSlice& output) { return this->write(output); };
  return FormatResult::write_any(write, output);
}

void ReplyHandshakeError::print(IMessagePrinter& printer) const
{
  MessagePrinting::print_fields(
    printer,
    "handshake_error",
    handshake_error
  );
}

}
