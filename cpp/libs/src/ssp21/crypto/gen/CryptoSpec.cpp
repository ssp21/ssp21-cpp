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

#include "ssp21/crypto/gen/CryptoSpec.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

CryptoSpec::CryptoSpec()
{}

CryptoSpec::CryptoSpec(
    NonceMode nonce_mode,
    DHMode dh_mode,
    HandshakeHash handshake_hash,
    HandshakeKDF handshake_kdf,
    HandshakeMAC handshake_mac,
    SessionMode session_mode
) :
    nonce_mode(nonce_mode),
    dh_mode(dh_mode),
    handshake_hash(handshake_hash),
    handshake_kdf(handshake_kdf),
    handshake_mac(handshake_mac),
    session_mode(session_mode)
{}

ParseError CryptoSpec::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        nonce_mode,
        dh_mode,
        handshake_hash,
        handshake_kdf,
        handshake_mac,
        session_mode
    );
}

FormatError CryptoSpec::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        nonce_mode,
        dh_mode,
        handshake_hash,
        handshake_kdf,
        handshake_mac,
        session_mode
    );
}

void CryptoSpec::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "nonce_mode",
        nonce_mode,
        "dh_mode",
        dh_mode,
        "handshake_hash",
        handshake_hash,
        "handshake_kdf",
        handshake_kdf,
        "handshake_mac",
        handshake_mac,
        "session_mode",
        session_mode
    );
}

}
