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

#ifndef SSP21_VERSION_H
#define SSP21_VERSION_H

#include <cstdint>
#include "ser4cpp/serialization/BigEndian.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct Version final 
{
    Version();

    Version(
        uint16_t major_version,
        uint16_t minor_version
    );

    size_t size() const;

    static const uint8_t fixed_size_bytes = 4;

    IntegerField<ser4cpp::UInt16> major_version;
    IntegerField<ser4cpp::UInt16> minor_version;

    ParseError read(seq32_t& input);
    ParseError read_all(const seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
