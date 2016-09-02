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

#ifndef SSP21_REPLYHANDSHAKEERROR_H
#define SSP21_REPLYHANDSHAKEERROR_H

#include "ssp21/gen/Function.h"
#include "ssp21/gen/HandshakeError.h"
#include "ssp21/crypto/IMessage.h"

namespace ssp21 {

struct ReplyHandshakeError : public IMessage, private openpal::Uncopyable
{
    ReplyHandshakeError();

    ReplyHandshakeError(
        HandshakeError handshake_error
    );

    virtual ParseError read_message(openpal::RSlice input) override;
    virtual FormatResult write_message(openpal::WSlice output) const override;
    virtual void print_message(IMessagePrinter& printer) const override;

    static const uint32_t fixed_size_bytes = 2;
    static const Function function = Function::reply_handshake_error;

    HandshakeError handshake_error;

};

}

#endif
