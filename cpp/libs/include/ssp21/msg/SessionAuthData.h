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

#ifndef SSP21_SESSIONAUTHDATA_H
#define SSP21_SESSIONAUTHDATA_H

#include <cstdint>
#include "openpal/util/Uncopyable.h"
#include "openpal/container/WSlice.h"
#include "openpal/container/RSlice.h"
#include "ssp21/gen/Function.h"
#include "ssp21/gen/ParseError.h"
#include "ssp21/gen/FormatError.h"
#include "ssp21/msg/PayloadFlags.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/FormatResult.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct SessionAuthData : public IReadable, public IWritable, public IPrintable, private openpal::Uncopyable
{
    SessionAuthData();

    SessionAuthData(
        uint32_t valid_until_ms,
        uint16_t nonce,
        const PayloadFlags& payload_flags
    );

    static const uint32_t min_size_bytes = 8;

    uint32_t valid_until_ms;
    uint16_t nonce;
    PayloadFlags payload_flags;

    virtual ParseError read(openpal::RSlice& input) override;

    virtual FormatError write(openpal::WSlice& output) const override;

    virtual void print(const char* name, IMessagePrinter& printer) const override;
};

}

#endif
