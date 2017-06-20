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

#include "ssp21/crypto/gen/CertificateBody.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

CertificateBody::CertificateBody()
{}

CertificateBody::CertificateBody(
    uint32_t valid_after,
    uint32_t valid_before,
    uint8_t signing_level,
    PublicKeyType public_key_type,
    const seq32_t& public_key
) :
    valid_after(valid_after),
    valid_before(valid_before),
    signing_level(signing_level),
    public_key_type(public_key_type),
    public_key(public_key)
{}

size_t CertificateBody::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        valid_after,
        valid_before,
        signing_level,
        public_key_type,
        public_key,
        extensions
    );
}

ParseError CertificateBody::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        valid_after,
        valid_before,
        signing_level,
        public_key_type,
        public_key,
        extensions
    );
}

ParseError CertificateBody::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError CertificateBody::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        valid_after,
        valid_before,
        signing_level,
        public_key_type,
        public_key,
        extensions
    );
}

void CertificateBody::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "valid_after",
        valid_after,
        "valid_before",
        valid_before,
        "signing_level",
        signing_level,
        "public_key_type",
        public_key_type,
        "public_key",
        public_key,
        "extensions",
        extensions
    );
}

}
