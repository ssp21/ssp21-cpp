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

#include "ssp21/msg/SessionAuthData.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

SessionAuthData::SessionAuthData() : 
    valid_until_ms(0),
    nonce(0)
{}

SessionAuthData::SessionAuthData(
    uint32_t valid_until_ms,
    uint16_t nonce,
    const PayloadFlags& payload_flags
) :
    valid_until_ms(valid_until_ms),
    nonce(nonce),
    payload_flags(payload_flags)
{}

ParseError SessionAuthData::read(openpal::RSlice& input)
{
    return MessageParser::read_fields(
        input,
        valid_until_ms,
        nonce,
        payload_flags
    );
}

FormatError SessionAuthData::write(openpal::WSlice& output) const
{
    return MessageFormatter::write_fields(
        output,
        valid_until_ms,
        nonce,
        payload_flags
    );
}

void SessionAuthData::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "valid_until_ms",
        valid_until_ms,
        "nonce",
        nonce,
        "payload_flags",
        payload_flags
    );
}

}
