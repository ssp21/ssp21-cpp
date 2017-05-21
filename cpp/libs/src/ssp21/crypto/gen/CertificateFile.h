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

#ifndef SSP21_CERTIFICATEFILE_H
#define SSP21_CERTIFICATEFILE_H

#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/CertificateFileEntry.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/SeqStructField.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct CertificateFile final 
{
    CertificateFile();


    static const uint8_t min_size_bytes = 1;

    SeqStructField<openpal::UInt8, CertificateFileEntry, 10> entries;

    ParseError read(seq32_t& input);
    ParseError read_all(const seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
