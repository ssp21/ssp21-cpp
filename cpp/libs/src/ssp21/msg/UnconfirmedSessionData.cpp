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

#include "ssp21/msg/UnconfirmedSessionData.h"

#include "ssp21/MessageParser.h"
#include "ssp21/MessageFormatter.h"

namespace ssp21 {

UnconfirmedSessionData::UnconfirmedSessionData() : 
  valid_until_ms(0),
  nonce(0)
{}

UnconfirmedSessionData::UnconfirmedSessionData(
  uint32_t valid_until_ms,
  uint16_t nonce,
  const Seq16& payload
) :
  valid_until_ms(valid_until_ms),
  nonce(nonce),
  payload(payload)
{}

ParseError UnconfirmedSessionData::read_msg(const openpal::RSlice& input)
{
  return MessageParser::read_message<Function::unconfirmed_session_data>(
    input,
    valid_until_ms,
    nonce,
    payload
  );
}

FormatError UnconfirmedSessionData::write(openpal::WSlice& output)
{
  return MessageFormatter::write_message<Function::unconfirmed_session_data>(
    output,
    valid_until_ms,
    nonce,
    payload
  );
}

FormatResult UnconfirmedSessionData::write_msg(openpal::WSlice& output)
{
  auto write = [this](openpal::WSlice& output) { return this->write(output); };
  return FormatResult::write_any(write, output);
}

}
