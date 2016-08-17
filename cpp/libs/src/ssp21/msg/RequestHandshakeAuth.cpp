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

#include "ssp21/msg/RequestHandshakeAuth.h"

#include "ssp21/MessageParser.h"
#include "ssp21/MessageFormatter.h"

namespace ssp21 {

RequestHandshakeAuth::RequestHandshakeAuth()
{
}

RequestHandshakeAuth::RequestHandshakeAuth(
  const Seq8& mac
) :
  mac(mac)
{}

ParseError RequestHandshakeAuth::read(openpal::RSlice& input)
{
  return MessageParser::read_message<Function::request_handshake_auth>(
    input,
    mac
  );
}

FormatError RequestHandshakeAuth::write(openpal::WSlice& output)
{
  return MessageFormatter::write_message<Function::request_handshake_auth>(
    output,
    mac
  );
}

}
