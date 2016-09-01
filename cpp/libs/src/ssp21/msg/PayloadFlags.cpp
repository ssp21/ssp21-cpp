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

#include "ssp21/msg/PayloadFlags.h"

#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/FlagsPrinting.h"

namespace ssp21 {

ParseError PayloadFlags::read(openpal::RSlice& input)
{
    uint8_t value = 0;
    if(!openpal::UInt8::read_from(input, value)) return ParseError::insufficient_bytes;

    fir = (value & 0x80) != 0;
    fin = (value & 0x40) != 0;

    return ParseError::ok;
}

FormatError PayloadFlags::write(openpal::WSlice& output) const
{
    uint8_t value = 0;

    if(fir) value |= 0x80;
    if(fin) value |= 0x40;

    return openpal::UInt8::write_to(output, value) ? FormatError::ok : FormatError::insufficient_space;
}

void PayloadFlags::print(const char* name, IMessagePrinter& printer) const
{
    FlagsPrinting::print(printer, name, "fir", fir, "fin", fin);
}

}
