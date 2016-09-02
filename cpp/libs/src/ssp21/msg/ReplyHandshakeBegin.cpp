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

#include "ssp21/msg/ReplyHandshakeBegin.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

ReplyHandshakeBegin::ReplyHandshakeBegin()
{}

ReplyHandshakeBegin::ReplyHandshakeBegin(
    const Seq8& ephemeral_public_key
) :
    ephemeral_public_key(ephemeral_public_key)
{}

ParseError ReplyHandshakeBegin::read(openpal::RSlice& input)
{
    return MessageParser::read_fields(
        input,
        ephemeral_public_key,
        certificates
    );
}

FormatError ReplyHandshakeBegin::write(openpal::WSlice& output) const
{
    return MessageFormatter::write_fields(
        output,
        ephemeral_public_key,
        certificates
    );
}

void ReplyHandshakeBegin::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "ephemeral_public_key",
        ephemeral_public_key,
        "certificates",
        certificates
    );
}

ParseError ReplyHandshakeBegin::read_message(openpal::RSlice input)
{
    return MessageParser::read_message<ReplyHandshakeBegin>(input, *this);
}

FormatResult ReplyHandshakeBegin::write_message(openpal::WSlice output) const
{
    return MessageFormatter::write_message<ReplyHandshakeBegin>(output, *this);
}
void ReplyHandshakeBegin::print_message(IMessagePrinter& printer) const
{
    return this->print("", printer);
}


}
