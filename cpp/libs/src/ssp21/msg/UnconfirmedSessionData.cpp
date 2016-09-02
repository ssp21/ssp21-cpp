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
    const AuthMetadata& metadata,
    const Seq16& payload
) :
    metadata(metadata),
    payload(payload)
{}


ParseError UnconfirmedSessionData::read(openpal::RSlice input)
{
    auto read_fields = [this](openpal::RSlice& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            metadata,
            payload
        );
    };

    return MessageParser::read_message(input, Function::unconfirmed_session_data, read_fields);
}

FormatResult UnconfirmedSessionData::write(openpal::WSlice output) const
{
    auto write_fields = [this](openpal::WSlice& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            metadata,
            payload
        );
    };

    return MessageFormatter::write_message(output, Function::unconfirmed_session_data, write_fields);
}
void UnconfirmedSessionData::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "metadata",
        metadata,
        "payload",
        payload
    );
}


}
