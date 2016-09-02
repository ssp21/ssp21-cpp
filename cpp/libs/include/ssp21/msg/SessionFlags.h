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

#include "ssp21/crypto/IMessageField.h"

namespace ssp21 {

struct SessionFlags final : public IMessageField
{
    SessionFlags(){}

    SessionFlags(bool fir, bool fin) :
        fir(fir),
        fin(fin)
    {}

    virtual ParseError read(openpal::RSlice& input) override;
    virtual FormatError write(openpal::WSlice& output) const override;
    virtual void print(const char* name, IMessagePrinter& printer) const override;

    bool fir = true;
    bool fin = true;
};

}

#endif
