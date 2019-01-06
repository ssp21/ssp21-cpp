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

#ifndef SSP21_AUTHMETADATA_H
#define SSP21_AUTHMETADATA_H

#include <cstdint>
#include "openpal/serialization/BigEndian.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct AuthMetadata final 
{
    AuthMetadata();

    AuthMetadata(
        uint16_t nonce,
        uint32_t valid_until_ms
    );

    size_t size() const;

    static const uint8_t fixed_size_bytes = 6;

    IntegerField<openpal::UInt16> nonce;
    IntegerField<openpal::UInt32> valid_until_ms;

    ParseError read(seq32_t& input);
    ParseError read_all(const seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
