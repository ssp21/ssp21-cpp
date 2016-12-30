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

#include "ssp21/crypto/gen/RequestHandshakeAuth.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

RequestHandshakeAuth::RequestHandshakeAuth()
{}

RequestHandshakeAuth::RequestHandshakeAuth(
    const seq8_t& mac
) :
    mac(mac)
{}


ParseError RequestHandshakeAuth::read(seq32_t input)
{
    auto read_fields = [this](seq32_t& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            mac
        );
    };

    return MessageParser::read_message(input, Function::request_handshake_auth, read_fields);
}

FormatResult RequestHandshakeAuth::write(wseq32_t& output) const
{
    auto write_fields = [this](wseq32_t& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            mac
        );
    };

    return MessageFormatter::write_message(output, Function::request_handshake_auth, write_fields);
}
void RequestHandshakeAuth::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "mac",
        mac
    );
}


}
