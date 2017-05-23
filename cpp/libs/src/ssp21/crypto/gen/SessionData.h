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

#ifndef SSP21_SESSIONDATA_H
#define SSP21_SESSIONDATA_H

#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/gen/AuthMetadata.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/SeqByteField.h"

namespace ssp21 {

struct SessionData final : public IMessage, private openpal::Uncopyable
{
    SessionData();

    SessionData(
        const AuthMetadata& metadata,
        const seq16_t& user_data,
        const seq8_t& auth_tag
    );

    size_t size() const;

    virtual ParseError read(seq32_t input) override;
    virtual FormatResult write(wseq32_t& output) const override;
    virtual void print(IMessagePrinter& printer) const override;
    virtual Function get_function() const override { return Function::session_data; }

    static const uint8_t min_size_bytes = 11;
    static const Function function = Function::session_data;

    AuthMetadata metadata;
    SeqByteField<openpal::UInt16> user_data;
    SeqByteField<openpal::UInt8> auth_tag;

};

}

#endif
