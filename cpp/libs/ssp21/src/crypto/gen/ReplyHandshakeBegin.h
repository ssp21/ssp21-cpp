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

#include "ssp21/crypto/SeqByteField.h"
#include "crypto/IMessage.h"
#include "crypto/gen/Function.h"

namespace ssp21 {

struct ReplyHandshakeBegin final : public IMessage, private ser4cpp::Uncopyable
{
    ReplyHandshakeBegin();

    ReplyHandshakeBegin(
        const seq32_t& ephemeral_data,
        const seq32_t& mode_data
    );

    size_t size() const;

    virtual ParseError read(seq32_t input) override;
    virtual FormatResult write(wseq32_t& output) const override;
    virtual void print(IMessagePrinter& printer) const override;
    virtual Function get_function() const override { return Function::reply_handshake_begin; }

    static const uint8_t min_size_bytes = 3;
    static const Function function = Function::reply_handshake_begin;

    SeqByteField ephemeral_data;
    SeqByteField mode_data;

};

}

#endif
