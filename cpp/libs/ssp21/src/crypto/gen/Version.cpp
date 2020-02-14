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
    uint16_t major_version,
    uint16_t minor_version
) :
    major_version(major_version),
    minor_version(minor_version)
{}

size_t Version::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        major_version,
        minor_version
    );
}

ParseError Version::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        major_version,
        minor_version
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
        major_version,
        minor_version
    );
}

void Version::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "major_version",
        major_version,
        "minor_version",
        minor_version
    );
}

}
