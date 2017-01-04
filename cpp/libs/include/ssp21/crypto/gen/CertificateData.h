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

#ifndef SSP21_CERTIFICATEDATA_H
#define SSP21_CERTIFICATEDATA_H

#include <cstdint>
#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/SequenceTypes.h"
#include "ssp21/crypto/SeqField.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct CertificateData final 
{
    CertificateData();

    CertificateData(
        uint32_t valid_after,
        uint32_t valid_before,
        const seq8_t& id,
        uint8_t role,
        uint8_t key_type,
        const seq8_t& public_key
    );

    static const uint32_t min_size_bytes = 12;

    IntegerField<openpal::UInt32> valid_after;
    IntegerField<openpal::UInt32> valid_before;
    SeqField<openpal::UInt8> id;
    IntegerField<openpal::UInt8> role;
    IntegerField<openpal::UInt8> key_type;
    SeqField<openpal::UInt8> public_key;

    ParseError read(seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
