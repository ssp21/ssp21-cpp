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

#ifndef SSP21_AUTHMETADATA_H
#define SSP21_AUTHMETADATA_H

#include <cstdint>
#include "ssp21/msg/SessionFlags.h"
#include "ssp21/crypto/IMessageField.h"

namespace ssp21 {

struct AuthMetadata : public IMessageField
{
    AuthMetadata();

    AuthMetadata(
        uint16_t nonce,
        uint32_t valid_until_ms,
        const SessionFlags& flags
    );

    static const uint32_t fixed_size_bytes = 7;

    uint16_t nonce;
    uint32_t valid_until_ms;
    SessionFlags flags;

    virtual ParseError read(openpal::RSlice& input) override;
    virtual FormatError write(openpal::WSlice& output) const override;
    virtual void print(const char* name, IMessagePrinter& printer) const override;
};

}

#endif
