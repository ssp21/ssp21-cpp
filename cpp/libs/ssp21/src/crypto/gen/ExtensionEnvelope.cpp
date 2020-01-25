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

#include "ssp21/crypto/gen/ExtensionEnvelope.h"

#include "crypto/MessageParser.h"
#include "crypto/MessagePrinting.h"
#include "crypto/MessageFormatter.h"

namespace ssp21 {

ExtensionEnvelope::ExtensionEnvelope()
{}

ExtensionEnvelope::ExtensionEnvelope(
    uint32_t identifier,
    const seq32_t& extension_body
) :
    identifier(identifier),
    extension_body(extension_body)
{}

size_t ExtensionEnvelope::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        identifier,
        extension_body
    );
}

ParseError ExtensionEnvelope::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        identifier,
        extension_body
    );
}

ParseError ExtensionEnvelope::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError ExtensionEnvelope::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        identifier,
        extension_body
    );
}

void ExtensionEnvelope::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "identifier",
        identifier,
        "extension_body",
        extension_body
    );
}

}
