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

#ifndef SSP21_EXTENSIONENVELOPE_H
#define SSP21_EXTENSIONENVELOPE_H

#include <cstdint>
#include "ser4cpp/serialization/BigEndian.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/SeqByteField.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct ExtensionEnvelope final 
{
    ExtensionEnvelope();

    ExtensionEnvelope(
        uint32_t identifier,
        const seq32_t& extension_body
    );

    size_t size() const;

    static const uint8_t min_size_bytes = 5;

    IntegerField<ser4cpp::UInt32> identifier;
    SeqByteField extension_body;

    ParseError read(seq32_t& input);
    ParseError read_all(const seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
