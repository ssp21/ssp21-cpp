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

#include "crypto/gen/CryptoSpec.h"

#include "crypto/MessageParser.h"
#include "crypto/MessagePrinting.h"
#include "crypto/MessageFormatter.h"

namespace ssp21 {

CryptoSpec::CryptoSpec()
{}

CryptoSpec::CryptoSpec(
    NonceMode nonce_mode,
    HandshakeEphemeral handshake_ephemeral,
    HandshakeHash handshake_hash,
    HandshakeKDF handshake_kdf,
    SessionMode session_mode
) :
    nonce_mode(nonce_mode),
    handshake_ephemeral(handshake_ephemeral),
    handshake_hash(handshake_hash),
    handshake_kdf(handshake_kdf),
    session_mode(session_mode)
{}

size_t CryptoSpec::size() const
{
    return MessageFormatter::sum_sizes(
        0,
        nonce_mode,
        handshake_ephemeral,
        handshake_hash,
        handshake_kdf,
        session_mode
    );
}

ParseError CryptoSpec::read(seq32_t& input)
{
    return MessageParser::read_fields(
        input,
        nonce_mode,
        handshake_ephemeral,
        handshake_hash,
        handshake_kdf,
        session_mode
    );
}

ParseError CryptoSpec::read_all(const seq32_t& input)
{
    auto remainder = input;
    auto err = read(remainder);
    if(any(err)) return err;
    return remainder.is_empty() ? ParseError::ok : ParseError::too_many_bytes;
}

FormatError CryptoSpec::write(wseq32_t& output) const
{
    return MessageFormatter::write_fields(
        output,
        nonce_mode,
        handshake_ephemeral,
        handshake_hash,
        handshake_kdf,
        session_mode
    );
}

void CryptoSpec::print(const char* name, IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "nonce_mode",
        nonce_mode,
        "handshake_ephemeral",
        handshake_ephemeral,
        "handshake_hash",
        handshake_hash,
        "handshake_kdf",
        handshake_kdf,
        "session_mode",
        session_mode
    );
}

}
