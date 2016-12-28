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
#include "ssp21/crypto/gen/Function.h"
#include "ssp21/crypto/gen/CertificateMode.h"
#include "ssp21/crypto/gen/CryptoSpec.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/SequenceTypes.h"

namespace ssp21 {

struct RequestHandshakeBegin final : public IMessage, private openpal::Uncopyable
{
    RequestHandshakeBegin();

    RequestHandshakeBegin(
        uint16_t version,
        const CryptoSpec& spec,
        CertificateMode certificate_mode,
        const Seq8& ephemeral_public_key
    );

    virtual ParseError read(openpal::RSlice input) override;
    virtual FormatResult write(openpal::WSlice output) const override;
    virtual void print(IMessagePrinter& printer) const override;

    static const uint32_t min_size_bytes = 12;
    static const Function function = Function::request_handshake_begin;

    IntegerField<openpal::UInt16> version;
    CryptoSpec spec;
    EnumField<CertificateModeSpec> certificate_mode;
    Seq8 ephemeral_public_key;
    Seq8Seq16 certificates;

};

}

#endif
