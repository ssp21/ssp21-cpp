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

AuthMetadata::AuthMetadata()
{}

AuthMetadata::AuthMetadata(
    uint16_t nonce,
    uint32_t valid_until_ms
) :
    nonce(nonce),
    valid_until_ms(valid_until_ms)
{}

size_t AuthMetadata::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        nonce,
        valid_until_ms
    );
}

ParseError AuthMetadata::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        nonce,
        valid_until_ms
    );
}

ParseError AuthMetadata::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError AuthMetadata::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        nonce,
        valid_until_ms
    );
}

void AuthMetadata::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "nonce",
        nonce,
        "valid_until_ms",
        valid_until_ms
    );
}

}
