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

#include "ssp21/crypto/EnumField.h"
#include "ssp21/util/SequenceTypes.h"
#include "ssp21/crypto/gen/NonceMode.h"
#include "ssp21/crypto/gen/ParseError.h"
#include "ssp21/crypto/gen/FormatError.h"
#include "ssp21/crypto/gen/SessionMode.h"
#include "ssp21/crypto/IMessagePrinter.h"
#include "ssp21/crypto/gen/HandshakeKDF.h"
#include "ssp21/crypto/gen/HandshakeHash.h"
#include "ssp21/crypto/gen/HandshakeEphemeral.h"

namespace ssp21 {

struct CryptoSpec final 
{
    CryptoSpec();

    CryptoSpec(
        NonceMode nonce_mode,
        HandshakeEphemeral handshake_ephemeral,
        HandshakeHash handshake_hash,
        HandshakeKDF handshake_kdf,
        SessionMode session_mode
    );

    size_t size() const;

    static const uint8_t fixed_size_bytes = 5;

    EnumField<NonceModeSpec> nonce_mode;
    EnumField<HandshakeEphemeralSpec> handshake_ephemeral;
    EnumField<HandshakeHashSpec> handshake_hash;
    EnumField<HandshakeKDFSpec> handshake_kdf;
    EnumField<SessionModeSpec> session_mode;

    ParseError read(seq32_t& input);
    ParseError read_all(const seq32_t& input);
    FormatError write(wseq32_t& output) const;
    void print(const char* name, IMessagePrinter& printer) const;
};

}

#endif
