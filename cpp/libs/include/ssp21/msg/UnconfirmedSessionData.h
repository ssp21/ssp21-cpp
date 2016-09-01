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

#ifndef SSP21_UNCONFIRMEDSESSIONDATA_H
#define SSP21_UNCONFIRMEDSESSIONDATA_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"
#include "openpal/container/WSlice.h"
#include "openpal/container/RSlice.h"
#include "ssp21/gen/Function.h"
#include "ssp21/gen/ParseError.h"
#include "ssp21/gen/FormatError.h"
#include "ssp21/msg/PayloadFlags.h"
#include "ssp21/crypto/FormatResult.h"
#include "ssp21/crypto/SequenceTypes.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct UnconfirmedSessionData : private openpal::Uncopyable
{
    UnconfirmedSessionData();

    UnconfirmedSessionData(
        uint32_t valid_until_ms,
        uint16_t nonce,
        const PayloadFlags& payload_flags,
        const Seq16& payload
    );

    ParseError read_msg(const openpal::RSlice& input);
    FormatResult write_msg(openpal::WSlice& output) const;

    void print(IMessagePrinter& printer) const;

    static const uint32_t min_size_bytes = 10;

    static const Function function = Function::unconfirmed_session_data;

    uint32_t valid_until_ms;
    uint16_t nonce;
    PayloadFlags payload_flags;
    Seq16 payload;

    private: 

    FormatError write(openpal::WSlice& output) const;
};

}

#endif
