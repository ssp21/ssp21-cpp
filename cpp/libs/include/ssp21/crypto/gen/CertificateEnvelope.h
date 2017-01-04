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

#ifndef SSP21_CERTIFICATEENVELOPE_H
#define SSP21_CERTIFICATEENVELOPE_H

#include <cstdint>
#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/SequenceTypes.h"
#include "ssp21/crypto/SeqField.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct CertificateEnvelope final 
{
    CertificateEnvelope();

    CertificateEnvelope(
        const seq8_t& certificate_data,
        const seq8_t& issuer_id,
        uint8_t signature_algorithm,
        const seq8_t& signature
    );

    static const uint32_t min_size_bytes = 4;

    SeqField<openpal::UInt8> certificate_data;
    SeqField<openpal::UInt8> issuer_id;
    IntegerField<openpal::UInt8> signature_algorithm;
    SeqField<openpal::UInt8> signature;

    ParseError read(seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
