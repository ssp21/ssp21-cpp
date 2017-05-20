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

#include "ssp21/crypto/gen/SessionConstraints.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

SessionConstraints::SessionConstraints()
{}

SessionConstraints::SessionConstraints(
    uint16_t max_nonce,
    uint32_t max_session_duration
) :
    max_nonce(max_nonce),
    max_session_duration(max_session_duration)
{}

ParseError SessionConstraints::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        max_nonce,
        max_session_duration
    );
}

ParseError SessionConstraints::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError SessionConstraints::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        max_nonce,
        max_session_duration
    );
}

void SessionConstraints::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "max_nonce",
        max_nonce,
        "max_session_duration",
        max_session_duration
    );
}

}
