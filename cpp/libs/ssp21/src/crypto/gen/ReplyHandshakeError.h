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

#ifndef SSP21_REPLYHANDSHAKEERROR_H
#define SSP21_REPLYHANDSHAKEERROR_H

#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/gen/HandshakeError.h"
#include "crypto/IMessage.h"
#include "crypto/gen/Version.h"
#include "crypto/gen/Function.h"

namespace ssp21 {

struct ReplyHandshakeError final : public IMessage
{
    ReplyHandshakeError();

    ReplyHandshakeError(
        const Version& version,
        HandshakeError handshake_error
    );

    size_t size() const;

    virtual ParseError read(seq32_t input) override;
    virtual FormatResult write(wseq32_t& output) const override;
    virtual void print(IMessagePrinter& printer) const override;
    virtual Function get_function() const override { return Function::reply_handshake_error; }

    static const uint8_t fixed_size_bytes = 6;
    static const Function function = Function::reply_handshake_error;

    Version version;
    EnumField<HandshakeErrorSpec> handshake_error;

};

}

#endif
