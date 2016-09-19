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

#include "ssp21/crypto/gen/Base64DecodeError.h"

namespace ssp21 {

const char* Base64DecodeErrorSpec::to_string(Base64DecodeError arg)
{
    switch(arg)
    {
        case(Base64DecodeError::ok):
            return "ok";
        case(Base64DecodeError::not_mult_four):
            return "not_mult_four";
        case(Base64DecodeError::not_base64):
            return "not_base64";
        case(Base64DecodeError::bad_trailing_input):
            return "bad_trailing_input";
        default:
            return "undefined";
    }
}

}
