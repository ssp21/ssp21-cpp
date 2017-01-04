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
    const seq8_t& certificate_data,
    const seq8_t& issuer_id,
    uint8_t signature_algorithm,
    const seq8_t& signature
) :
    certificate_data(certificate_data),
    issuer_id(issuer_id),
    signature_algorithm(signature_algorithm),
    signature(signature)
{}

ParseError CertificateEnvelope::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        certificate_data,
        issuer_id,
        signature_algorithm,
        signature
    );
}

FormatError CertificateEnvelope::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        certificate_data,
        issuer_id,
        signature_algorithm,
        signature
    );
}

void CertificateEnvelope::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "certificate_data",
        certificate_data,
        "issuer_id",
        issuer_id,
        "signature_algorithm",
        signature_algorithm,
        "signature",
        signature
    );
}

}
