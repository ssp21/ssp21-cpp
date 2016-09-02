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

#include "ssp21/msg/RequestHandshakeAuth.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

RequestHandshakeAuth::RequestHandshakeAuth()
{}

RequestHandshakeAuth::RequestHandshakeAuth(
    const Seq8& mac
) :
    mac(mac)
{}

ParseError RequestHandshakeAuth::read(openpal::RSlice& input)
{
    return MessageParser::read_fields(
        input,
        mac
    );
}

FormatError RequestHandshakeAuth::write(openpal::WSlice& output) const
{
    return MessageFormatter::write_fields(
        output,
        mac
    );
}

void RequestHandshakeAuth::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "mac",
        mac
    );
}

ParseError RequestHandshakeAuth::read_message(openpal::RSlice input)
{
    return MessageParser::read_message<RequestHandshakeAuth>(input, *this);
}

FormatResult RequestHandshakeAuth::write_message(openpal::WSlice output) const
{
    return MessageFormatter::write_message<RequestHandshakeAuth>(output, *this);
}
void RequestHandshakeAuth::print_message(IMessagePrinter& printer) const
{
    return this->print("", printer);
}


}
