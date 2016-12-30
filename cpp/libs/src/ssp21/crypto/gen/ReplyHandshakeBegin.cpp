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

#include "ssp21/crypto/gen/ReplyHandshakeBegin.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeBegin::ReplyHandshakeBegin()
{}

ReplyHandshakeBegin::ReplyHandshakeBegin(
    const seq8_t& ephemeral_public_key
) :
    ephemeral_public_key(ephemeral_public_key)
{}


ParseError ReplyHandshakeBegin::read(openpal::RSlice input)
{
    auto read_fields = [this](openpal::RSlice& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            ephemeral_public_key,
            certificates
        );
    };

    return MessageParser::read_message(input, Function::reply_handshake_begin, read_fields);
}

FormatResult ReplyHandshakeBegin::write(openpal::WSlice output) const
{
    auto write_fields = [this](openpal::WSlice& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            ephemeral_public_key,
            certificates
        );
    };

    return MessageFormatter::write_message(output, Function::reply_handshake_begin, write_fields);
}
void ReplyHandshakeBegin::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "ephemeral_public_key",
        ephemeral_public_key,
        "certificates",
        certificates
    );
}


}
