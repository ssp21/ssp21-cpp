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

#ifndef SSP21_CRYPTOSPEC_H
#define SSP21_CRYPTOSPEC_H

#include "ssp21/crypto/gen/DHMode.h"
#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/SessionMode.h"
#include "ssp21/crypto/gen/HandshakeKDF.h"
#include "ssp21/crypto/gen/HandshakeMAC.h"
#include "ssp21/crypto/gen/HandshakeHash.h"
#include "ssp21/SequenceTypes.h"
#include "ssp21/crypto/EnumField.h"
#include "ssp21/crypto/IMessagePrinter.h"

namespace ssp21 {

struct CryptoSpec final 
{
    CryptoSpec();

    CryptoSpec(
        NonceMode nonce_mode,
        DHMode dh_mode,
        HandshakeHash handshake_hash,
        HandshakeKDF handshake_kdf,
        HandshakeMAC handshake_mac,
        SessionMode session_mode
    );

    static const uint32_t fixed_size_bytes = 6;

    EnumField<NonceModeSpec> nonce_mode;
    EnumField<DHModeSpec> dh_mode;
    EnumField<HandshakeHashSpec> handshake_hash;
    EnumField<HandshakeKDFSpec> handshake_kdf;
    EnumField<HandshakeMACSpec> handshake_mac;
    EnumField<SessionModeSpec> session_mode;

    ParseError read(seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
