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

#include "ssp21/crypto/gen/ReassemblyResult.h"

namespace ssp21 {

const char* ReassemblyResultSpec::to_string(ReassemblyResult arg)
{
    switch(arg)
    {
        case(ReassemblyResult::complete):
            return "complete";
        case(ReassemblyResult::partial):
            return "partial";
        case(ReassemblyResult::bad_nonce):
            return "bad_nonce";
        default:
            return "undefined";
    }
}

}
