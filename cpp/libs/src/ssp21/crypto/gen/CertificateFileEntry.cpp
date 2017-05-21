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

#include "ssp21/crypto/gen/CertificateFileEntry.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

CertificateFileEntry::CertificateFileEntry()
{}

CertificateFileEntry::CertificateFileEntry(
    FileEntryType file_entry_type,
    const seq16_t& data
) :
    file_entry_type(file_entry_type),
    data(data)
{}

ParseError CertificateFileEntry::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        file_entry_type,
        data
    );
}

ParseError CertificateFileEntry::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError CertificateFileEntry::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        file_entry_type,
        data
    );
}

void CertificateFileEntry::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "file_entry_type",
        file_entry_type,
        "data",
        data
    );
}

}
