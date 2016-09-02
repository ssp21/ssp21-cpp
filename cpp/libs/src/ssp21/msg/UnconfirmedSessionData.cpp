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

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

UnconfirmedSessionData::UnconfirmedSessionData()
{}

UnconfirmedSessionData::UnconfirmedSessionData(
    const SessionAuthData& ad,
    const Seq16& payload
) :
    ad(ad),
    payload(payload)
{}

ParseError UnconfirmedSessionData::read_msg(const openpal::RSlice& input)
{
    return MessageParser::read_message<Function::unconfirmed_session_data>(
        input,
        ad,
        payload
    );
}

FormatError UnconfirmedSessionData::write(openpal::WSlice& output) const
{
    return MessageFormatter::write_message<Function::unconfirmed_session_data>(
        output,
        ad,
        payload
    );
}

FormatResult UnconfirmedSessionData::write_msg(openpal::WSlice& output) const
{
    auto write = [this](openpal::WSlice& output) { return this->write(output); };
    return FormatResult::write_any(write, output);
}

void UnconfirmedSessionData::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "ad",
        ad,
        "payload",
        payload
    );
}

}
