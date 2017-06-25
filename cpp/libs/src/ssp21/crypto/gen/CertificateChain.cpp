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

#include "ssp21/crypto/gen/CertificateChain.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

CertificateChain::CertificateChain()
{}


size_t CertificateChain::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        certificates
    );
}

ParseError CertificateChain::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        certificates
    );
}

ParseError CertificateChain::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError CertificateChain::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        certificates
    );
}

void CertificateChain::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "certificates",
        certificates
    );
}

}
