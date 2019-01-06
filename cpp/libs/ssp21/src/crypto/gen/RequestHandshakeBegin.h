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

#ifndef SSP21_REQUESTHANDSHAKEBEGIN_H
#define SSP21_REQUESTHANDSHAKEBEGIN_H

#include <cstdint>
#include "openpal/serialization/BigEndian.h"
#include "ssp21/crypto/gen/HandshakeMode.h"
#include "crypto/IMessage.h"
#include "crypto/EnumField.h"
#include "crypto/gen/Function.h"
#include "crypto/SeqByteField.h"
#include "crypto/IntegerField.h"
#include "crypto/gen/CryptoSpec.h"
#include "crypto/gen/SessionConstraints.h"

namespace ssp21 {

struct RequestHandshakeBegin final : public IMessage, private openpal::Uncopyable
{
    RequestHandshakeBegin();

    RequestHandshakeBegin(
        uint16_t version,
        const CryptoSpec& spec,
        const SessionConstraints& constraints,
        HandshakeMode handshake_mode,
        const seq32_t& ephemeral_data,
        const seq32_t& handshake_data
    );

    size_t size() const;

    virtual ParseError read(seq32_t input) override;
    virtual FormatResult write(wseq32_t& output) const override;
    virtual void print(IMessagePrinter& printer) const override;
    virtual Function get_function() const override { return Function::request_handshake_begin; }

    static const uint8_t min_size_bytes = 17;
    static const Function function = Function::request_handshake_begin;

    IntegerField<openpal::UInt16> version;
    CryptoSpec spec;
    SessionConstraints constraints;
    EnumField<HandshakeModeSpec> handshake_mode;
    SeqByteField ephemeral_data;
    SeqByteField handshake_data;

};

}

#endif
