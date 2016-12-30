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
// Licensed under the terms of the BSDv3 license
//

#include "ssp21/crypto/gen/SessionData.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

SessionData::SessionData()
{}

SessionData::SessionData(
    const AuthMetadata& metadata,
    const seq16_t& user_data,
    const seq8_t& auth_tag
) :
    metadata(metadata),
    user_data(user_data),
    auth_tag(auth_tag)
{}


ParseError SessionData::read(seq32_t input)
{
    auto read_fields = [this](seq32_t& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            metadata,
            user_data,
            auth_tag
        );
    };

    return MessageParser::read_message(input, Function::session_data, read_fields);
}

FormatResult SessionData::write(wseq32_t output) const
{
    auto write_fields = [this](wseq32_t& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            metadata,
            user_data,
            auth_tag
        );
    };

    return MessageFormatter::write_message(output, Function::session_data, write_fields);
}
void SessionData::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "metadata",
        metadata,
        "user_data",
        user_data,
        "auth_tag",
        auth_tag
    );
}


}
