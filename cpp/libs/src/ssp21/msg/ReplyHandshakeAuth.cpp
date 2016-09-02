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

#include "ssp21/msg/ReplyHandshakeAuth.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeAuth::ReplyHandshakeAuth()
{}

ReplyHandshakeAuth::ReplyHandshakeAuth(
    const Seq8& mac
) :
    mac(mac)
{}


ParseError ReplyHandshakeAuth::read(openpal::RSlice input)
{
    auto read_fields = [this](openpal::RSlice& input) -> ParseError 
    {
        return MessageParser::read_fields(
            input,
            mac
        );
    };

    return MessageParser::read_message(input, Function::request_handshake_auth, read_fields);
}

FormatResult ReplyHandshakeAuth::write(openpal::WSlice output) const
{
    auto write_fields = [this](openpal::WSlice& output) -> FormatError 
    {
        return MessageFormatter::write_fields(
            output,
            mac
        );
    };

    return MessageFormatter::write_message(output, Function::request_handshake_auth, write_fields);
}
void ReplyHandshakeAuth::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "mac",
        mac
    );
}


}
