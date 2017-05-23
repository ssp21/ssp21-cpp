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

#ifndef SSP21_REPLYHANDSHAKEAUTH_H
#define SSP21_REPLYHANDSHAKEAUTH_H

#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/SeqByteField.h"

namespace ssp21 {

struct ReplyHandshakeAuth final : public IMessage, private openpal::Uncopyable
{
    ReplyHandshakeAuth();

    explicit ReplyHandshakeAuth(
        const seq8_t& mac
    );

    size_t size() const;

    virtual ParseError read(seq32_t input) override;
    virtual FormatResult write(wseq32_t& output) const override;
    virtual void print(IMessagePrinter& printer) const override;
    virtual Function get_function() const override { return Function::reply_handshake_auth; }

    static const uint8_t min_size_bytes = 2;
    static const Function function = Function::reply_handshake_auth;

    SeqByteField<openpal::UInt8> mac;

};

}

#endif
