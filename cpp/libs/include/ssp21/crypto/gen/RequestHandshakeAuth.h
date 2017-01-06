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

#ifndef SSP21_REQUESTHANDSHAKEAUTH_H
#define SSP21_REQUESTHANDSHAKEAUTH_H

#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/SeqField.h"

namespace ssp21 {

struct RequestHandshakeAuth final : public IMessage, private openpal::Uncopyable
{
    RequestHandshakeAuth();

    explicit RequestHandshakeAuth(
        const seq8_t& mac
    );

    virtual ParseError read(seq32_t input) override;
    virtual FormatResult write(wseq32_t& output) const override;
    virtual void print(IMessagePrinter& printer) const override;

    static const uint8_t min_size_bytes = 2;
    static const Function function = Function::request_handshake_auth;

    SeqField<openpal::UInt8> mac;

};

}

#endif
