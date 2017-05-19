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

#ifndef SSP21_CERTIFICATEBODY_H
#define SSP21_CERTIFICATEBODY_H

#include <cstdint>
#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/PublicKeyType.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/SeqField.h"
#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/SeqSeqField.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct CertificateBody final 
{
    CertificateBody();

    CertificateBody(
        uint32_t serial_number,
        uint32_t valid_after,
        uint32_t valid_before,
        const seq8_t& signing_level,
        PublicKeyType public_key_type,
        const seq8_t& public_key
    );

    static const uint8_t min_size_bytes = 16;

    IntegerField<openpal::UInt32> serial_number;
    IntegerField<openpal::UInt32> valid_after;
    IntegerField<openpal::UInt32> valid_before;
    SeqField<openpal::UInt8> signing_level;
    EnumField<PublicKeyTypeSpec> public_key_type;
    SeqField<openpal::UInt8> public_key;
    SeqSeqField<openpal::UInt8, openpal::UInt16, 5> extensions;

    ParseError read(seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif