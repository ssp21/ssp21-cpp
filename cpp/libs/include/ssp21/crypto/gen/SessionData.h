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
#include "ssp21/crypto/SequenceFieldTypes.h"

namespace ssp21 {

struct SessionData final : public IMessage, private openpal::Uncopyable
{
    SessionData();

    SessionData(
        const AuthMetadata& metadata,
        const Seq16& user_data,
        const Seq8& auth_tag
    );

    virtual ParseError read(openpal::RSlice input) override;
    virtual FormatResult write(openpal::WSlice output) const override;
    virtual void print(IMessagePrinter& printer) const override;

    static const uint32_t min_size_bytes = 11;
    static const Function function = Function::session_data;

    AuthMetadata metadata;
    Seq16Field user_data;
    Seq8Field auth_tag;

};

}

#endif
