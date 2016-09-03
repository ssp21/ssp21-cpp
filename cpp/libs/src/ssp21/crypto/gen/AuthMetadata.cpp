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

#include "ssp21/crypto/gen/AuthMetadata.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

AuthMetadata::AuthMetadata() : 
    nonce(0),
    valid_until_ms(0)
{}

AuthMetadata::AuthMetadata(
    uint16_t nonce,
    uint32_t valid_until_ms,
    const SessionFlags& flags
) :
    nonce(nonce),
    valid_until_ms(valid_until_ms),
    flags(flags)
{}

ParseError AuthMetadata::read(openpal::RSlice& input)
{
    return MessageParser::read_fields(
        input,
        nonce,
        valid_until_ms,
        flags
    );
}

FormatError AuthMetadata::write(openpal::WSlice& output) const
{
    return MessageFormatter::write_fields(
        output,
        nonce,
        valid_until_ms,
        flags
    );
}

void AuthMetadata::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "nonce",
        nonce,
        "valid_until_ms",
        valid_until_ms,
        "flags",
        flags
    );
}

}
