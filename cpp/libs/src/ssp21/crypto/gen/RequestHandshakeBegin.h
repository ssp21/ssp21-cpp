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
#include "ssp21/crypto/gen/SessionConstraints.h"
#include "ssp21/crypto/gen/CertificateEnvelope.h"
#include "ssp21/crypto/IMessage.h"
#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/SeqByteField.h"
#include "ssp21/crypto/IntegerField.h"
#include "ssp21/crypto/SeqStructField.h"

namespace ssp21 {

struct RequestHandshakeBegin final : public IMessage, private openpal::Uncopyable
{
    RequestHandshakeBegin();

    RequestHandshakeBegin(
        uint16_t version,
        const CryptoSpec& spec,
        const SessionConstraints& constraints,
        CertificateMode certificate_mode,
        const seq8_t& ephemeral_public_key
    );

    size_t size() const;

    virtual ParseError read(seq32_t input) override;
    virtual FormatResult write(wseq32_t& output) const override;
    virtual void print(IMessagePrinter& printer) const override;
    virtual Function get_function() const override { return Function::request_handshake_begin; }

    static const uint8_t min_size_bytes = 18;
    static const Function function = Function::request_handshake_begin;

    IntegerField<openpal::UInt16> version;
    CryptoSpec spec;
    SessionConstraints constraints;
    EnumField<CertificateModeSpec> certificate_mode;
    SeqByteField<openpal::UInt8> ephemeral_public_key;
    SeqStructField<openpal::UInt8, CertificateEnvelope, 6> certificates;

};

}

#endif
