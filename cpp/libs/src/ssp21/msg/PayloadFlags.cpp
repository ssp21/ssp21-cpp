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

namespace ssp21 {

ParseError PayloadFlags::read(openpal::RSlice& input)
{
    if(input.is_empty()) return ParseError::insufficient_bytes;

    const auto value = input[0];
    input.advance(1);

    fir = (value & 0x80) != 0;
    fin = (value & 0x40) != 0;

    return ParseError::ok;
}

FormatError PayloadFlags::write(openpal::WSlice& output)
{
    if(output.is_empty()) return FormatError::insufficient_space;

    uint8_t value = 0;

    if(fir) value |= 0x80;
    if(fin) value |= 0x40;

    output[0] = value;
    output.advance(1);

    return FormatError::ok;
}

}
