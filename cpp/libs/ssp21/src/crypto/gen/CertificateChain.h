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

#ifndef SSP21_CERTIFICATECHAIN_H
#define SSP21_CERTIFICATECHAIN_H

#include "ssp21/util/SequenceTypes.h"
#include "crypto/gen/ParseError.h"
#include "crypto/SeqStructField.h"
#include "crypto/gen/FormatError.h"
#include "ssp21/crypto/IMessagePrinter.h"
#include "crypto/gen/CertificateEnvelope.h"

namespace ssp21 {

struct CertificateChain final 
{
    CertificateChain();


    size_t size() const;

    static const uint8_t min_size_bytes = 1;

    SeqStructField<CertificateEnvelope, 6> certificates;

    ParseError read(seq32_t& input);
    ParseError read_all(const seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
