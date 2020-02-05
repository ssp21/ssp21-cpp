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

#include "crypto/gen/Version.h"

#include "crypto/MessageParser.h"
#include "crypto/MessagePrinting.h"
#include "crypto/MessageFormatter.h"

namespace ssp21 {

Version::Version()
{}

Version::Version(
    uint16_t major,
    uint16_t minor
) :
    major(major),
    minor(minor)
{}

size_t Version::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        major,
        minor
    );
}

ParseError Version::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        major,
        minor
    );
}

ParseError Version::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError Version::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        major,
        minor
    );
}

void Version::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "major",
        major,
        "minor",
        minor
    );
}

}
