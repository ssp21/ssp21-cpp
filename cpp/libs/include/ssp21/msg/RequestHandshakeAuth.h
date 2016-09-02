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

#include "ssp21/gen/Function.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/SequenceTypes.h"

namespace ssp21 {

struct RequestHandshakeAuth : public IMessage, private openpal::Uncopyable
{
    RequestHandshakeAuth();

    RequestHandshakeAuth(
        const Seq8& mac
    );

    virtual ParseError read(openpal::RSlice input) override;
    virtual FormatResult write(openpal::WSlice output) const override;
    virtual void print(IMessagePrinter& printer) const override;

    static const uint32_t min_size_bytes = 2;
    static const Function function = Function::request_handshake_auth;

    Seq8 mac;

};

}

#endif
