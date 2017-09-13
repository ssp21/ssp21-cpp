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
    const seq32_t& ephemeral_data,
    const seq32_t& handshake_data
) :
    ephemeral_data(ephemeral_data),
    handshake_data(handshake_data)
{}

size_t ReplyHandshakeBegin::size() const
{
    return MessageFormatter::sum_sizes(
        1,
        ephemeral_data,
        handshake_data
    );
}


ParseError ReplyHandshakeBegin::read(seq32_t input)
{
    auto read_fields = [this](seq32_t& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            ephemeral_data,
            handshake_data
        );
    };

    return MessageParser::read_message(input, Function::reply_handshake_begin, read_fields);
}

FormatResult ReplyHandshakeBegin::write(wseq32_t& output) const
{
    auto write_fields = [this](wseq32_t& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            ephemeral_data,
            handshake_data
        );
    };

    return MessageFormatter::write_message(output, Function::reply_handshake_begin, write_fields);
}
void ReplyHandshakeBegin::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "ephemeral_data",
        ephemeral_data,
        "handshake_data",
        handshake_data
    );
}


}
