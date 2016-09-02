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

#ifndef SSP21_REPLYHANDSHAKEAUTH_H
#define SSP21_REPLYHANDSHAKEAUTH_H

#include "ssp21/gen/Function.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/SequenceTypes.h"

namespace ssp21 {

struct ReplyHandshakeAuth : public IMessage, private openpal::Uncopyable
{
    friend class MessageParser;
    friend class MessageFormatter;

    ReplyHandshakeAuth();

    ReplyHandshakeAuth(
        const Seq8& mac
    );

    virtual ParseError read_message(openpal::RSlice input) override;
    virtual FormatResult write_message(openpal::WSlice output) const override;
    virtual void print_message(IMessagePrinter& printer) const override;

    static const uint32_t min_size_bytes = 2;
    static const Function function = Function::request_handshake_auth;

    Seq8 mac;

    private:

    ParseError read(openpal::RSlice& input);
    FormatError write(openpal::WSlice& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
