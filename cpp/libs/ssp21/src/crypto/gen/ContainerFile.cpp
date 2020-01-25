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

#include "ssp21/crypto/gen/ContainerFile.h"

#include "crypto/MessageParser.h"
#include "crypto/MessagePrinting.h"
#include "crypto/MessageFormatter.h"

namespace ssp21 {

ContainerFile::ContainerFile()
{}

ContainerFile::ContainerFile(
    ContainerEntryType container_entry_type,
    const seq32_t& payload
) :
    container_entry_type(container_entry_type),
    payload(payload)
{}

size_t ContainerFile::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        container_entry_type,
        payload
    );
}

ParseError ContainerFile::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        container_entry_type,
        payload
    );
}

ParseError ContainerFile::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError ContainerFile::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        container_entry_type,
        payload
    );
}

void ContainerFile::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "container_entry_type",
        container_entry_type,
        "payload",
        payload
    );
}

}
