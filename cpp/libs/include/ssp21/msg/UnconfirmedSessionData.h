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

#ifndef SSP21_UNCONFIRMEDSESSIONDATA_H
#define SSP21_UNCONFIRMEDSESSIONDATA_H

#include "ssp21/gen/Function.h"
#include "ssp21/msg/AuthMetadata.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/SequenceTypes.h"

namespace ssp21 {

struct UnconfirmedSessionData : public IMessage, private openpal::Uncopyable
{
    friend class MessageParser;
    friend class MessageFormatter;

    UnconfirmedSessionData();

    UnconfirmedSessionData(
        const AuthMetadata& metadata,
        const Seq16& payload
    );

    virtual ParseError read_message(openpal::RSlice input) override;
    virtual FormatResult write_message(openpal::WSlice output) const override;
    virtual void print_message(IMessagePrinter& printer) const override;

    static const uint32_t min_size_bytes = 10;
    static const Function function = Function::unconfirmed_session_data;

    AuthMetadata metadata;
    Seq16 payload;

    private:

    ParseError read(openpal::RSlice& input);
    FormatError write(openpal::WSlice& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
