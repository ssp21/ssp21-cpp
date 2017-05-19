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

#ifndef SSP21_SESSIONFLAGS_H
#define SSP21_SESSIONFLAGS_H

#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct SessionFlags final
{
    SessionFlags(){}

    SessionFlags(bool fir, bool fin) :
        fir(fir),
        fin(fin)
    {}

    ParseError read(seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;

    bool fir = true;
    bool fin = true;
};

}

#endif
