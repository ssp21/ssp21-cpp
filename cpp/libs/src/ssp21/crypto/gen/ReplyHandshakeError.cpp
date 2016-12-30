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

#include "ssp21/crypto/gen/ReplyHandshakeError.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeError::ReplyHandshakeError()
{}

ReplyHandshakeError::ReplyHandshakeError(
    HandshakeError handshake_error
) :
    handshake_error(handshake_error)
{}


ParseError ReplyHandshakeError::read(seq32_t input)
{
    auto read_fields = [this](seq32_t& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            handshake_error
        );
    };

    return MessageParser::read_message(input, Function::reply_handshake_error, read_fields);
}

FormatResult ReplyHandshakeError::write(wseq32_t& output) const
{
    auto write_fields = [this](wseq32_t& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            handshake_error
        );
    };

    return MessageFormatter::write_message(output, Function::reply_handshake_error, write_fields);
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
