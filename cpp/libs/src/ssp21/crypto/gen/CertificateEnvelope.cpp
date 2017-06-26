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

#include "ssp21/crypto/gen/CertificateEnvelope.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

CertificateEnvelope::CertificateEnvelope()
{}

CertificateEnvelope::CertificateEnvelope(
    const seq32_t& signature,
    const seq32_t& certificate_body
) :
    signature(signature),
    certificate_body(certificate_body)
{}

size_t CertificateEnvelope::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        signature,
        certificate_body
    );
}

ParseError CertificateEnvelope::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        signature,
        certificate_body
    );
}

ParseError CertificateEnvelope::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError CertificateEnvelope::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        signature,
        certificate_body
    );
}

void CertificateEnvelope::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "signature",
        signature,
        "certificate_body",
        certificate_body
    );
}

}
