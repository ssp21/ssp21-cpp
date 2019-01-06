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

#include "ssp21/crypto/gen/PEMDecodeError.h"

namespace ssp21 {

const char* PEMDecodeErrorSpec::name = "PEM decode error";

const char* PEMDecodeErrorSpec::to_string(PEMDecodeError arg)
{
    switch(arg)
    {
        case(PEMDecodeError::ok):
            return "ok";
        case(PEMDecodeError::end_of_file):
            return "end_of_file";
        case(PEMDecodeError::missing_begin_header):
            return "missing_begin_header";
        default:
            return "undefined";
    }
}

}
