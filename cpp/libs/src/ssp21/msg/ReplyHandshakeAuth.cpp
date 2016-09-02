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

ParseError ReplyHandshakeAuth::read(openpal::RSlice& input)
{
    return MessageParser::read_fields(
        input,
        mac
    );
}

FormatError ReplyHandshakeAuth::write(openpal::WSlice& output) const
{
    return MessageFormatter::write_fields(
        output,
        mac
    );
}

void ReplyHandshakeAuth::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "mac",
        mac
    );
}

ParseError ReplyHandshakeAuth::read_message(openpal::RSlice input)
{
    return MessageParser::read_message<ReplyHandshakeAuth>(input, *this);
}

FormatResult ReplyHandshakeAuth::write_message(openpal::WSlice output) const
{
    return MessageFormatter::write_message<ReplyHandshakeAuth>(output, *this);
}
void ReplyHandshakeAuth::print_message(IMessagePrinter& printer) const
{
    return this->print("", printer);
}


}
