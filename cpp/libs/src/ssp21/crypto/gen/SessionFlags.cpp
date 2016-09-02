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

#include "ssp21/msg/SessionFlags.h"

#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/FlagsPrinting.h"

namespace ssp21 {

ParseError SessionFlags::read(openpal::RSlice& input)
{
    uint8_t value = 0;
    if(!openpal::UInt8::read_from(input, value)) return ParseError::insufficient_bytes;

    if((value & 0x3F) != 0) return ParseError::reserved_bit;

    fir = (value & 0x80) != 0;
    fin = (value & 0x40) != 0;

    return ParseError::ok;
}

FormatError SessionFlags::write(openpal::WSlice& output) const
{
    uint8_t value = 0;

    if(fir) value |= 0x80;
    if(fin) value |= 0x40;

    return openpal::UInt8::write_to(output, value) ? FormatError::ok : FormatError::insufficient_space;
}

void SessionFlags::print(const char* name, IMessagePrinter& printer) const
{
    FlagsPrinting::print(printer, name, "fir", fir, "fin", fin);
}

}
