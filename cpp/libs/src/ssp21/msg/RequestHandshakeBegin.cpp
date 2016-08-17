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

#include "ssp21/MessageParser.h"
#include "ssp21/MessageFormatter.h"

namespace ssp21 {

RequestHandshakeBegin::RequestHandshakeBegin() : 
  version(0),
  nonce_mode(NonceMode::undefined),
  dh_mode(DHMode::undefined),
  hash_mode(HashMode::undefined),
  session_mode(SessionMode::undefined),
  certificate_mode(CertificateMode::undefined)
{
}

ParseError RequestHandshakeBegin::read(openpal::RSlice& input)
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

FormatError RequestHandshakeBegin::write(openpal::WSlice& dest)
{
  return MessageFormatter::write_message<Function::request_handshake_begin>(
    dest,
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

}
