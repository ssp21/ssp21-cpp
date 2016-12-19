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
#include "openpal/container/WSlice.h"
#include "openpal/container/RSlice.h"
#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/SessionFlags.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct AuthMetadata final 
{
    AuthMetadata();

    AuthMetadata(
        uint16_t nonce,
        uint32_t valid_until_ms,
        const SessionFlags& flags
    );

    static const uint32_t fixed_size_bytes = 7;

    IntegerField<openpal::UInt16> nonce;
    IntegerField<openpal::UInt32> valid_until_ms;
    SessionFlags flags;

    ParseError read(openpal::RSlice& input);
    FormatError write(openpal::WSlice& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
