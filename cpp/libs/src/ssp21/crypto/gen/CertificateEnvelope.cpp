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
    const seq8_t& issuer_id,
    const seq8_t& signature,
    const seq16_t& certificate_body
) :
    issuer_id(issuer_id),
    signature(signature),
    certificate_body(certificate_body)
{}

ParseError CertificateEnvelope::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        issuer_id,
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
        issuer_id,
        signature,
        certificate_body
    );
}

void CertificateEnvelope::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "issuer_id",
        issuer_id,
        "signature",
        signature,
        "certificate_body",
        certificate_body
    );
}

}
