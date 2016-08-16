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

namespace ssp21 {

UnconfirmedSessionData::UnconfirmedSessionData() : 
  valid_until_ms(0),
  nonce(0)
{
}

ParseError UnconfirmedSessionData::read(openpal::RSlice& input)
{
  return MessageParser::read_message<Function::unconfirmed_session_data>(
    input,
    valid_until_ms,
    nonce,
    payload
  );
}

}
