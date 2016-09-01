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
// License TBD
//

#ifndef SSP21_PAYLOADFLAGS_H
#define SSP21_PAYLOADFLAGS_H

#include "ssp21/crypto/IReadable.h"
#include "ssp21/crypto/IWritable.h"

namespace ssp21 {

struct PayloadFlags final : public IReadable, public IWritable
{
    PayloadFlags(){}

    PayloadFlags(bool fir, bool fin) :
        fir(fir),
        fin(fin)
    {}

    virtual ParseError read(openpal::RSlice& input) override;
    virtual FormatError write(openpal::WSlice& output) override;

    bool fir = true;
    bool fin = true;
};

}

#endif
