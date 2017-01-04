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

#include "ssp21/crypto/gen/CertificateData.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

CertificateData::CertificateData()
{}

CertificateData::CertificateData(
    uint32_t valid_after,
    uint32_t valid_before,
    const seq8_t& id,
    uint8_t role,
    uint8_t key_type,
    const seq8_t& public_key
) :
    valid_after(valid_after),
    valid_before(valid_before),
    id(id),
    role(role),
    key_type(key_type),
    public_key(public_key)
{}

ParseError CertificateData::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        valid_after,
        valid_before,
        id,
        role,
        key_type,
        public_key
    );
}

FormatError CertificateData::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        valid_after,
        valid_before,
        id,
        role,
        key_type,
        public_key
    );
}

void CertificateData::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "valid_after",
        valid_after,
        "valid_before",
        valid_before,
        "id",
        id,
        "role",
        role,
        "key_type",
        key_type,
        "public_key",
        public_key
    );
}

}
