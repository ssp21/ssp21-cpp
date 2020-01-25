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
#include "ser4cpp/serialization/BigEndian.h"
#include "ssp21/crypto/EnumField.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/SeqByteField.h"
#include "ssp21/crypto/SeqStructField.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/gen/PublicKeyType.h"
#include "ssp21/crypto/gen/ExtensionEnvelope.h"

namespace ssp21 {

struct CertificateBody final 
{
    CertificateBody();

    CertificateBody(
        uint64_t valid_after,
        uint64_t valid_before,
        uint8_t signing_level,
        PublicKeyType public_key_type,
        const seq32_t& public_key
    );

    size_t size() const;

    static const uint8_t min_size_bytes = 20;

    IntegerField<ser4cpp::UInt64> valid_after;
    IntegerField<ser4cpp::UInt64> valid_before;
    IntegerField<ser4cpp::UInt8> signing_level;
    EnumField<PublicKeyTypeSpec> public_key_type;
    SeqByteField public_key;
    SeqStructField<ExtensionEnvelope, 5> extensions;

    ParseError read(seq32_t& input);
    ParseError read_all(const seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
