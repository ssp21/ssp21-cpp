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

#ifndef SSP21_REASSEMBLYRESULT_H
#define SSP21_REASSEMBLYRESULT_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"

namespace ssp21 {

/**
    The result of a message reassembly operation
*/
enum class ReassemblyResult : uint8_t
{
    /// reassembly complete
    complete = 0x0,
    /// partial reassembly. more data needed
    partial = 0x1,
    /// reassembly failed due to an out-of-order nonce
    bad_nonce = 0x2,
    /// reassembly failed because a non-fir packet was received before receiving a fir packet
    no_prior_fir = 0x3,
    /// reassembly failed the new packet would overflow the reassembly buffer
    overflow = 0x4
};

struct ReassemblyResultSpec : private openpal::StaticOnly
{
    typedef ReassemblyResult enum_type_t;

    static const char* to_string(ReassemblyResult arg);
};

}

#endif
