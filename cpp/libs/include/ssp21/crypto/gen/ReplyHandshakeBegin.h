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

#ifndef SSP21_REPLYHANDSHAKEBEGIN_H
#define SSP21_REPLYHANDSHAKEBEGIN_H

#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/SeqField.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/SeqSeqField.h"

namespace ssp21 {

struct ReplyHandshakeBegin final : public IMessage, private openpal::Uncopyable
{
    ReplyHandshakeBegin();

    explicit ReplyHandshakeBegin(
        const Seq8& ephemeral_public_key
    );

    virtual ParseError read(openpal::RSlice input) override;
    virtual FormatResult write(openpal::WSlice output) const override;
    virtual void print(IMessagePrinter& printer) const override;

    static const uint32_t min_size_bytes = 3;
    static const Function function = Function::reply_handshake_begin;

    SeqField<openpal::UInt8> ephemeral_public_key;
    Seq8Seq16Field certificates;

};

}

#endif
