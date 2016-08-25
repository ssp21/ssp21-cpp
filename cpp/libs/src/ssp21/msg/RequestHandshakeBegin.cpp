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

#include "ssp21/msg/RequestHandshakeBegin.h"

#include "ssp21/crypto/MessageParser.h"
#include "ssp21/crypto/MessagePrinting.h"
#include "ssp21/crypto/MessageFormatter.h"

namespace ssp21 {

RequestHandshakeBegin::RequestHandshakeBegin() : 
    version(0),
    nonce_mode(NonceMode::undefined),
    dh_mode(DHMode::undefined),
    hash_mode(HashMode::undefined),
    session_mode(SessionMode::undefined),
    certificate_mode(CertificateMode::undefined)
{}

RequestHandshakeBegin::RequestHandshakeBegin(
    uint16_t version,
    NonceMode nonce_mode,
    DHMode dh_mode,
    HashMode hash_mode,
    SessionMode session_mode,
    CertificateMode certificate_mode,
    const Seq8& ephemeral_public_key
) :
    version(version),
    nonce_mode(nonce_mode),
    dh_mode(dh_mode),
    hash_mode(hash_mode),
    session_mode(session_mode),
    certificate_mode(certificate_mode),
    ephemeral_public_key(ephemeral_public_key)
{}

ParseError RequestHandshakeBegin::read_msg(const openpal::RSlice& input)
{
    return MessageParser::read_message<Function::request_handshake_begin>(
        input,
        version,
        nonce_mode,
        dh_mode,
        hash_mode,
        session_mode,
        certificate_mode,
        ephemeral_public_key,
        certificates
    );
}

FormatError RequestHandshakeBegin::write(openpal::WSlice& output) const
{
    return MessageFormatter::write_message<Function::request_handshake_begin>(
        output,
        version,
        nonce_mode,
        dh_mode,
        hash_mode,
        session_mode,
        certificate_mode,
        ephemeral_public_key,
        certificates
    );
}

FormatResult RequestHandshakeBegin::write_msg(openpal::WSlice& output) const
{
    auto write = [this](openpal::WSlice& output) { return this->write(output); };
    return FormatResult::write_any(write, output);
}

void RequestHandshakeBegin::print(IMessagePrinter& printer) const
{
    MessagePrinting::print_fields(
        printer,
        "version",
        version,
        "nonce_mode",
        nonce_mode,
        "dh_mode",
        dh_mode,
        "hash_mode",
        hash_mode,
        "session_mode",
        session_mode,
        "certificate_mode",
        certificate_mode,
        "ephemeral_public_key",
        ephemeral_public_key,
        "certificates",
        certificates
    );
}

}
